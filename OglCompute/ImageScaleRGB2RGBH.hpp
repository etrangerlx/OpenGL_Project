#ifndef __ImageScaleRGB2RGBH_HPP__
#define __ImageScaleRGB2RGBH_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"
#include "OglBaseMem.hpp"
#define DEBUG__PRINT
#if defined(DEBUG__PRINT)
#define PRINT {\
    printf("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
    LOGI("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
}
#else
#define PRINT 
#endif

enum IMAGRGBH_FORMAT
{
    E_ImageRGBHScale = 0,
    E_ImageScale_RGB2RGBH = 1,
    E_ImageScale_RGBH2RGB = 2
};

class ImageRGB2GBHRender : public Renderer
{
public:
    ImageRGB2GBHRender();
    ~ImageRGB2GBHRender();
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
    virtual bool initUseCS();
	virtual void render();
    virtual void renderUseCS();
    void setInputTexture(OglTexture *tex);

    void setOutputTexture(OglTexture *tex);
    void setRotation(float rotation)
    {
        float a = cos(rotation);
        float b = sin(rotation);
        matrix2x2[0][0] = a;
        matrix2x2[0][1] = b;
        matrix2x2[1][0] = -b;
        matrix2x2[1][1] = a;
        
    }
    void setOutputSSBO(OglBaseMem* ssbo) { m_ssbo = ssbo; }
    void setInputSSBO(OglBaseMem* ssbo) { m_ssbo = ssbo; }
    void setRenderType(int type)
    {
        m_RenderType = type;
    }

	virtual	void deinit();

private:
    Shader      *m_ScaleShader=nullptr;
    Shader      *m_ScaleRgb2RgbhShader=nullptr;
    Shader      *m_ScaleRgbh2RgbShader=nullptr;
    OglTexture  *m_input;
    OglTexture  *m_output;
    OglBaseMem* m_ssbo = nullptr;
    int         m_RenderType;
    glm::mat2 matrix2x2={1.0f,0.0f,0.0f,1.0f}; 
    FrameBufferObject m_fbo;
};
#endif