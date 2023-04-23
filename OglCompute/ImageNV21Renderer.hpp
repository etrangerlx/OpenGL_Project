#ifndef __ImageNV21Renderer_HPP__
#define __ImageNV21Renderer_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"

enum IMAGESCALENV21_FORMAT
{
    E_ImageScale2Y = 0,
    E_ImageScale2NV = 1
};

class ImageNV21Renderer : public Renderer
{
public:
    ImageNV21Renderer();

    void readTex(OglTexture *tex,char * OutBuf)
    {
        m_fbo.attachTexture(tex);
        m_fbo.bind();

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(0, 0, tex->getWidth(), tex->getHeight(),
            tex->getTextureFormat(), GL_UNSIGNED_BYTE, OutBuf);

        m_fbo.attachTexture(m_output);
        m_fbo.bind();
    }
    
    virtual bool init();

	virtual void render();

    void setInputTexture(OglTexture *tex);
    void setOutputTexture(OglTexture *tex);
    void setRenderType(int type)
    {
        m_RenderType = type;
    }

	virtual	void deinit();

private:
    Shader      *m_Scale2YShader;
    Shader      *m_Scale2NVShader;
    OglTexture  *m_input;
    OglTexture  *m_output;
    int         m_RenderType;

    FrameBufferObject m_fbo;
};
#endif