#include "ImageResampleRenderer.hpp"

class ResampleShader : public Shader
{
public:
    ResampleShader()
    {    
            m_fs = 
            "uniform sampler2D tex0;\n\
            uniform sampler2D textBGR;\n\
            uniform float scalex;\n\
            uniform float scaley;\n\
            uniform float scalez;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                ivec2 tabSize = textureSize(textBGR,0);\n\
                vec3 colorIn = (texture(tex0,texcoord)).rgb;\n\
                vec2 gf = texcoord*vec2(float(imgSize.x),float(imgSize.y))*vec2(scalex,scaley);\n\
                vec2 g0=floor(gf);\n\
                vec3 gz0 = colorIn*255.0*scalez;\n\
                vec3 gintz = floor(gz0);\n\
                float step_x = 1.0/float(tabSize.x);\n\
                float step_y = 1.0/float(tabSize.y);\n\
                float sx=0.0,sy=0.0,sz=0.0;\n\
                vec3 rgb=vec3(0.0,0.0,0.0);\n\
                for(sy=(g0.y);sy<(g0.y)+2.0;sy = sy+1.0){\n\
                    float wy=1.0-abs((sy)-gf.y);\n\
                    float sy1 = sy*step_y+0.5*step_y;\n\
                    for(sx=(g0.x);sx<(g0.x)+2.0f;sx=sx+1.0){\n\
                        float wx = 1.0-abs(sx-gf.x);\n\
                        float wxy = wx*wy;\n\
                        vec3 wzbgr;\n\
                        vec3 gintz1 = gintz;\n\
                        //for(int i=0;i<2;i++) \n\
                        {\n\
                            wzbgr = 1.0-abs(gintz1-gz0);\n\
                            vec3 idxxbgr = (sx*10.0+gintz1+0.5)*step_x;\n\
                            vec2 idxb = vec2(idxxbgr.z,sy1);\n\
                            vec2 idxg = vec2(idxxbgr.y,sy1+0.333333);\n\
                            vec2 idxr = vec2(idxxbgr.x,sy1+0.666667);\n\
                            rgb = rgb + wxy*wzbgr*vec3(texture(textBGR,idxr).x,texture(textBGR,idxg).x,texture(textBGR,idxb).x);\n\
                            gintz1 = gintz1+1.0;\n\
                            \n\
                            wzbgr = 1.0-abs(gintz1-gz0);\n\
                            idxxbgr = (sx*10.0+gintz1+0.5)*step_x;\n\
                            idxb = vec2(idxxbgr.z,sy1);\n\
                            idxg = vec2(idxxbgr.y,sy1+0.333333);\n\
                            idxr = vec2(idxxbgr.x,sy1+0.666667);\n\
                            rgb = rgb + wxy*wzbgr*vec3(texture(textBGR,idxr).x,texture(textBGR,idxg).x,texture(textBGR,idxb).x);\n\
                        }\n\
                    }\n\
                }\n\
                color=vec4(rgb,1.0f);\n\
                //color=texture(tex0,texcoord)*scalez;\n\
            }"; 

            m_vs = 
        #ifdef OpenGLES 
            "#version 320 es\n\
            precision lowp float;\n"
        #else
            "#version 330\n"
        #endif
            "layout(location = 0) in vec3 in_vertex;\n\
            layout(location = 1) in vec2 in_texcoord;\n\
            out vec2 texcoord;\n\
            void main(){\n\
                gl_Position = vec4(in_vertex, 1);\n\
                texcoord = in_texcoord;\n\
            }";       
    }
};

ImageResampleRenderer::ImageResampleRenderer() 
                : m_ResampleShader(nullptr)
{
    
}

ImageResampleRenderer::~ImageResampleRenderer() 
{
    deinit();
}
bool ImageResampleRenderer::init()
{
    initVao();
	checkGLError();

    m_ResampleShader = new ResampleShader();
    m_ResampleShader->compile();
	checkGLError();
    printf("m_ScaleShader->compile\n");CHECK;
    return true;
}

void ImageResampleRenderer::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void ImageResampleRenderer::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageResampleRenderer::setTabTextureR(OglTexture *tex)
{
        m_inputR = tex;
}
void ImageResampleRenderer::setTabTextureG(OglTexture *tex)
{
        m_inputG = tex;
}
void ImageResampleRenderer::setTabTextureB(OglTexture *tex)
{
        m_inputB = tex;
}

void ImageResampleRenderer::setTabTextureBGR(OglTexture *tex)
{
        m_inputBGR = tex;
}

void ImageResampleRenderer::render()
{
    // downscale
    
    //glDisable(GL_BLEND);
	checkGLError();
    m_fbo.attachTexture(m_output);
    m_fbo.bind();
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight());
    glClearColor(0.0, 0.0, 0.0, 1.0);
    m_ResampleShader->use();
    m_ResampleShader->setInt("tex0", 0);
    m_ResampleShader->setInt("textBGR", 1);CHECK;
    
    glActiveTexture(GL_TEXTURE0);CHECK;
    m_input->bind();CHECK;
    glActiveTexture(GL_TEXTURE1);CHECK;
    m_inputBGR->bind();CHECK;

    glBindVertexArray(m_vao);CHECK;
    m_ResampleShader->setFloat("scalex",scalex);
    m_ResampleShader->setFloat("scaley",scaley);
    m_ResampleShader->setFloat("scalez",scalez);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);CHECK;
    glBindVertexArray(0);CHECK;
    m_fbo.unbind();
    m_input->unbind();
    m_inputBGR->unbind();
    glFinish();
	checkGLError();

}

void ImageResampleRenderer::deinit()
{
    if (m_ResampleShader)
    {
        delete m_ResampleShader;
        m_ResampleShader = NULL;
    }
    deinitVao();
}
