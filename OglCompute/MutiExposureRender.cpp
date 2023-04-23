#include "MutiExposureRender.hpp"

class MutiExPosureShader : public Shader
{
public:
    MutiExPosureShader()
    {
        m_fs = 
            "uniform lowp sampler2D tex0;\n\
            uniform lowp sampler2D tex1;\n\
            uniform lowp sampler2D w0;\n\
            precision highp float;\n\
            precision lowp  sampler2D; \n\
            in vec2 texcoord;\n\
            out ivec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 color1 = texture(tex0, texcoord);\n\
                vec4 color2 = texture(tex1, texcoord);\n\
                vec4 weight = texture(w0,texcoord+vec2(xStep*0.5,0.0));\n\
                vec4 rst = color1*weight.x+color2*weight.y;\n\
                rst.a = 1.0;\n\
                float sumWeight = weight.x+weight.y;\n\
                vec4 color4 = 255.0f*rst/(sumWeight);\n\
                color = ivec4(int(color4.r),int(color4.g),int(color4.b),int(color4.a));\n\
            }";
    }
};

class MutiExPosureShader16 : public Shader
{
public:
    MutiExPosureShader16()
    {
        m_fs = 
            "uniform lowp isampler2D tex0;\n\
            uniform lowp isampler2D tex1;\n\
            uniform lowp sampler2D w0;\n\
            precision highp float;\n\
            precision lowp  sampler2D; \n\
            in vec2 texcoord;\n\
            out ivec4 color;\n\
            void main() {\n\
                ivec4 color1 = texture(tex0, texcoord);\n\
                ivec4 color2 = texture(tex1, texcoord);\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 weight = texture(w0,texcoord);\n\
                vec2 iweight= vec2(float(weight.x*255.0),float(weight.y*255.0));\n\
                vec3 rst = vec3(float(color1.r),float(color1.g),float(color1.b))*iweight.x+vec3(float(color2.r),float(color2.g),float(color2.b))*iweight.y;\n\
                float sumWeight = iweight.x+iweight.y;\n\
                vec3 color3 =rst/(sumWeight);\n\
                color = ivec4(int(color3.r),int(color3.g),int(color3.b),255);\n\
            }";
    }
};


class ImageScaleShader16 : public Shader
{
public:
    ImageScaleShader16()
    {
        m_fs = 
            "uniform lowp isampler2D tex0;\n\
            in vec2 texcoord;\n\
            out ivec4 color;\n\
            void main() {\n\
                ivec4 color1 = texture(tex0, texcoord);\n\
                ivec4 color4 = color1;\n\
                color = color4;\n\
            }";
    }
};

class SubtractByteEdgeShader : public Shader
{
public:
    SubtractByteEdgeShader()
    {
        m_fs = 
            "uniform lowp sampler2D tex0;\n\
            uniform lowp sampler2D tex1;\n\
            in vec2 texcoord;\n\
            out ivec4 color;\n\
            void main() {\n\
                vec4 color1 = texture(tex0, texcoord);\n\
                vec4 color2 = texture(tex1, texcoord);\n\
                vec4 color4 = 255.0f*(color1-color2);\n\
                color = ivec4(int(color4.r),int(color4.g),int(color4.b),int(color4.a));\n\
            }";
    }
};


class AddBackEdgeShader : public Shader
{
public:
    AddBackEdgeShader()
    {
        m_fs = 
            "uniform lowp isampler2D tex0;\n\
            uniform lowp isampler2D tex1;\n\
            in vec2 texcoord;\n\
            out ivec4 color;\n\
            void main() {\n\
                ivec4 color1 = texture(tex0, texcoord);\n\
                ivec4 color2 = texture(tex1, texcoord);\n\
                ivec4 color4 = (color1+color2);\n\
                color = color4;\n\
            }";
    }
};

class GaussFilter146 : public Shader
{
public:
    GaussFilter146()
    {
         m_fs =
            "uniform lowp isampler2D tex0;\n\
            in vec2 texcoord;\n\
            out ivec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                ivec4 sum = texture(tex0, vec2(texcoord.x - 0.5 * xStep, texcoord.y - 1.5 * yStep)) * 50\n\
                        + texture(tex0, vec2(texcoord.x - 1.5 * xStep, texcoord.y + 0.5 * yStep)) * 50\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 50\n\
                        + texture(tex0, vec2(texcoord.x + 1.5 * xStep, texcoord.y - 0.5 * yStep)) * 50\n\
                        + texture(tex0, vec2(texcoord.x + 0.5 * xStep, texcoord.y + 1.5 * yStep)) * 50;\n\
                color = int(sum / 50);\n\
            }";
    }
};

MutiExPosureRender::MutiExPosureRender() 
                : m_FusionImageShader(nullptr)
                , m_RenderType(E_FusionImage)
{
    
}

bool MutiExPosureRender::init()
{
    initVao();
	checkGLError();

  //  m_FusionImageShader = new MutiExPosureShader();
  //  m_FusionImageShader->compile();
	//checkGLError();
    m_FusionImageShader16 = new MutiExPosureShader16();
    m_FusionImageShader16->compile();

 //   m_SubtractByteEdgeShader = new SubtractByteEdgeShader();
 //   m_SubtractByteEdgeShader->compile();

 //   m_AddBackEdgeShader = new AddBackEdgeShader();
 //   m_AddBackEdgeShader->compile();
  //  checkGLError();
   
 //  m_ScaleImageShader = new ImageScaleShader16();
 //  m_ScaleImageShader->compile();
    printf(" m_Scaleyuv2RgbShader->compile sucessed\n");
    return true;
}

void MutiExPosureRender::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void MutiExPosureRender::setInputTexture1(OglTexture *tex)
{
    m_input1 = tex;
}

void MutiExPosureRender::setInputTexture2(OglTexture *tex)
{
    m_input2 = tex;
}

void MutiExPosureRender::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void MutiExPosureRender::render()
{
    CHECK;
    m_fbo.attachTexture(m_output); CHECK;
    m_fbo.bind(); CHECK;
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight()); CHECK;
    glClearColor(0.0, 0.0, 0.0, 1.0); CHECK;
    glBindVertexArray(m_vao);CHECK;
   
    if (m_RenderType == E_FusionImage)
    {
        m_FusionImageShader->use();
        m_FusionImageShader->setInt("tex0", 0);
        m_FusionImageShader->setInt("tex1", 1);
        m_FusionImageShader->setInt("w0", 2);
        glActiveTexture(GL_TEXTURE0);CHECK;
        m_input->bind();CHECK;
        glActiveTexture(GL_TEXTURE1);
        m_input1->bind();CHECK;
        glActiveTexture(GL_TEXTURE2);
        m_input2->bind();CHECK;  
    }
    else if (m_RenderType == E_FusionImage16)
    {
        m_FusionImageShader16->use();
        m_FusionImageShader16->setInt("tex0", 0);
        m_FusionImageShader16->setInt("tex1", 1);
        m_FusionImageShader16->setInt("w0", 2);
        glActiveTexture(GL_TEXTURE0);CHECK;
        m_input->bind();CHECK;
        glActiveTexture(GL_TEXTURE1);
        m_input1->bind();CHECK;
        glActiveTexture(GL_TEXTURE2);
        m_input2->bind();CHECK;  
    }
    else if(m_RenderType == E_Subtract16)
    {
        m_SubtractByteEdgeShader->use();
        m_SubtractByteEdgeShader->setInt("tex0", 0);
        m_SubtractByteEdgeShader->setInt("tex1", 1);
        glActiveTexture(GL_TEXTURE0);CHECK;
        m_input->bind();CHECK;
        glActiveTexture(GL_TEXTURE1);
        m_input1->bind();CHECK;
    }
    else if(m_RenderType == E_AddBack16)
    {
        m_AddBackEdgeShader->use();
        m_AddBackEdgeShader->setInt("tex0", 0);
        m_AddBackEdgeShader->setInt("tex1", 1);
        glActiveTexture(GL_TEXTURE0);CHECK;
        m_input->bind();CHECK;
        glActiveTexture(GL_TEXTURE1);
        m_input1->bind();CHECK;
    }
    else if(m_RenderType == E_Scale16)
    {
        m_ScaleImageShader->use();
        m_ScaleImageShader->setInt("tex0", 0);
        glActiveTexture(GL_TEXTURE0);CHECK;
        m_input->bind();CHECK;
    }
    else
    {
        printf("error !!! e_type not support !!! \n\n");
    }
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);CHECK;
    glBindVertexArray(0);CHECK;
    m_fbo.unbind();
    if(m_input!=nullptr)
    {
        m_input->unbind();
    }
    if(m_input1!=nullptr)
    {
        m_input1->unbind();
    }
    if(m_input2!=nullptr)
    {
        m_input2->unbind();
    }
    glFinish();
	checkGLError();

}

void MutiExPosureRender::deinit()
{
    if (m_FusionImageShader)
    {
        delete m_FusionImageShader;
        m_FusionImageShader = NULL;
    }
    if(m_SubtractByteEdgeShader)
    {
        delete m_SubtractByteEdgeShader;
        m_SubtractByteEdgeShader = NULL;
    }
    if (m_FusionImageShader16)
    {
        delete m_FusionImageShader16;
        m_FusionImageShader16 = NULL;
    }
    if(m_AddBackEdgeShader)
    {
        delete m_AddBackEdgeShader;
        m_AddBackEdgeShader = NULL;
    }
    if(m_ScaleImageShader)
    {
        delete m_ScaleImageShader;
        m_ScaleImageShader = NULL;
    }
    deinitVao();
}
