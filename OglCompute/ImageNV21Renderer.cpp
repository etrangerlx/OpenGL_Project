#include "ImageNV21Renderer.hpp"
#define CHECK                                                                                                           \
    {   \
        glFinish() ;                                                                                                           \
        GLenum error = glGetError();                                                                                    \
        if (error != GL_NO_ERROR ){                                                             \
            printf("error !! line = %d gl code: %d, file: %s,func: %s \n", __LINE__, error, __FILE__, __FUNCTION__); \
            LOGI("error !! line = %d gl code: %d,func: %s \n", __LINE__, error, __FUNCTION__);\
        }\
    }


class Scale2NVShader : public Shader
{
public:
    Scale2NVShader()
    {
        m_fs = 
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            vec3 rgb2yuv(vec3 rgb)\n\
            {\n\
                vec3 yuv; \n\
                yuv.x = 0.299 * rgb.x + 0.587 * rgb.y + 0.114 * rgb.z; \n\
                yuv.y = (rgb.z - yuv.x)/1.772 + 0.5f; \n\
                yuv.z = (rgb.x - yuv.x)/1.402 + 0.5f; \n\
                return yuv; \n\
            }\n\
            void main() {\n\
                color = texture(tex0, vec2(texcoord.x,texcoord.y));\n\
                color.rgb = rgb2yuv(color.rgb);\n\
                color.a = 1.0f;\n\
            }";
    }
};


//Y = 0.299*R + 0.587*G + 0.114*B;
//U = (B - Y) / 1.772 + 0.5;
//V = (R - Y) / 1.402 + 0.5;  

class Scale2YShader : public Shader
{
public:
    Scale2YShader()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            vec3 rgb2yuv(vec3 rgb)    \n\
            {\n\
                vec3 yuv; \n\
                yuv.x = 0.299 * rgb.x + 0.587 * rgb.y + 0.114 * rgb.z; \n\
                yuv.y=yuv.x;\n\
                yuv.z=yuv.x;\n\
                return yuv; \n\
            }\n\
            void main() {\n\
                vec3 yuv;\n\
                color = texture(tex0, texcoord);\n\
                yuv = rgb2yuv(color.rgb);\n\
                color.xyz = yuv.xyz;\n\
                color.a=1.0f;\n\
            }";
    }
};

ImageNV21Renderer::ImageNV21Renderer() 
                : m_Scale2NVShader(nullptr)
                , m_Scale2YShader(nullptr)
                , m_RenderType(E_ImageScale2Y)
{
    
}

bool ImageNV21Renderer::init()
{
    glDisable(GL_BLEND);
    initVao();
	checkGLError();
    
    m_Scale2YShader = new Scale2YShader();
    m_Scale2YShader->compile();
	checkGLError();
    printf("m_Scale2YShader->compile()\n");
    m_Scale2NVShader = new Scale2NVShader();
    m_Scale2NVShader->compile();
    checkGLError();
    printf("m_Scale2NVShader->compile()\n");
    return true;
}

void ImageNV21Renderer::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void ImageNV21Renderer::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageNV21Renderer::render()
{
    // downscale
	checkGLError();
    m_fbo.attachTexture(m_output);
    m_fbo.bind();
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight());
    glClearColor(0.0, 0.0, 0.0, 1.0);

    if (m_RenderType == E_ImageScale2NV)
    {
        m_Scale2NVShader->use();
        m_Scale2NVShader->setInt("tex0", 0);
    }
    else
    {
        m_Scale2YShader->use();
        m_Scale2YShader->setInt("tex0", 0);
    }
/*
    glBindVertexArray(m_vao);CHECK;
    glActiveTexture(GL_TEXTURE0);CHECK;
    m_input->bind();CHECK;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);CHECK;
    glBindVertexArray(0);CHECK;
    m_fbo.unbind();
    glFinish();
	checkGLError();
*/
    glBindVertexArray(m_vao);CHECK;
    glActiveTexture(GL_TEXTURE0);CHECK;
    m_input->bind();CHECK;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);CHECK;
    glBindVertexArray(0);CHECK;
    m_fbo.unbind();CHECK;
    glFinish();
	checkGLError();
}

void ImageNV21Renderer::deinit()
{
    if (m_Scale2NVShader)
    {
        delete m_Scale2NVShader;
        m_Scale2NVShader = NULL;
    }
    if (m_Scale2YShader)
    {
        delete m_Scale2YShader;
        m_Scale2YShader = NULL;
    }
    deinitVao();
}
