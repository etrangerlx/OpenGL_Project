#ifndef __IMAGEBLENDRENDERER_HPP__
#define __IMAGEBLENDRENDERER_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"

enum IMAGEBLEND_FORMAT
{
    E_ImageBlend = 0,
    E_Bilateral,
    E_Conv3H,
    E_Conv3V,
    E_Conv5H,
    E_Conv5V,
    E_Conv7H,
    E_Conv7V,
    E_Conv9H,
    E_Conv9V,
    E_Conv3x3Opt,
    E_Conv5x5Opt,
    E_Conv7x7Opt,
    E_Conv9x9Opt
};

class ImageBlendRenderer : public Renderer
{
public:
    ImageBlendRenderer();
    
    virtual bool init();

	virtual void render();

    void setInput0Texture(OglTexture *tex);

    void setInput1Texture(OglTexture *tex);

    void setInput2Texture(OglTexture *tex);

    void setOutputTexture(OglTexture *tex);

    void setMaskTrans(float dx, float dy);

    void setRenderType(int type)
    {
        m_RenderType = type;
    }

	virtual	void deinit();

private:
    Shader      *m_bilateralShader;
	Shader      *m_blendShader;
    
    Shader      *m_conv3HShader;
    Shader      *m_conv3VShader;

    Shader      *m_conv5HShader;
    Shader      *m_conv5VShader;

    Shader      *m_conv7HShader;
    Shader      *m_conv7VShader;

    Shader      *m_conv9HShader;
    Shader      *m_conv9VShader;

    Shader      *m_conv3x3OptShader;
    Shader      *m_conv5x5OptShader;
    Shader      *m_conv7x7OptShader;
    Shader      *m_conv9x9OptShader;

    int         m_RenderType;

    OglTexture  *m_input0;
    OglTexture  *m_input1;
    OglTexture  *m_input2;
    OglTexture  *m_output;

    glm::vec2   m_maskTrans;

    FrameBufferObject m_fbo;
};
#endif