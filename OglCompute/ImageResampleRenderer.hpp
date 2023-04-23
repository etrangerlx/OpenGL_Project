#ifndef __ImageResampleRenderer_HPP__
#define __ImageResampleRenderer_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"


class ImageResampleRenderer : public Renderer
{
public:
    ImageResampleRenderer();
    ~ImageResampleRenderer();
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
    void setTabTextureR(OglTexture *tex);
    void setTabTextureG(OglTexture *tex);
    void setTabTextureB(OglTexture *tex);
    void setTabTextureBGR(OglTexture *tex);
    void setScale(float x,float y,float z)
    {
      scalex = x;
      scaley = y;
      scalez = z;  
    }
	virtual	void deinit();

private:
    Shader      *m_ResampleShader;
    OglTexture  *m_input;
    OglTexture  *m_inputR;
    OglTexture  *m_inputG;
    OglTexture  *m_inputB;
    OglTexture  *m_inputBGR;
    float scalex;
    float scaley;
    float scalez;
    OglTexture  *m_output;

    FrameBufferObject m_fbo;
};
#endif