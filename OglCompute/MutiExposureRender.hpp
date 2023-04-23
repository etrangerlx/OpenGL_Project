#ifndef __MutiExposureRender_HPP__
#define __MutiExposureRender_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"

//#define DEBUG__PRINT
#if defined(DEBUG__PRINT)
#define PRINT {\
    printf("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
}
#else
#define PRINT 
#endif

enum HDR_ALG
{
    E_FusionImage = 0,
    E_FusionImage16 = 1,

    E_Subtract = 2,
    E_Subtract16 = 3,

    E_AddBack = 4,
    E_AddBack16 = 5,
    E_Scale16 = 6,
    E_UpScale = 7
};

class MutiExPosureRender : public Renderer
{
public:
    MutiExPosureRender();

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
    void setInputTexture1(OglTexture *tex);
    void setInputTexture2(OglTexture *tex);
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
    void setRenderType(int type)
    {
        m_RenderType = type;
    }

	virtual	void deinit();

private:
    Shader      *m_FusionImageShader;
    Shader      *m_FusionImageShader16=nullptr;
    Shader      *m_SubtractByteEdgeShader = nullptr;
    Shader      *m_AddBackEdgeShader= nullptr;
    Shader      *m_ScaleImageShader = nullptr;
    OglTexture  *m_input=nullptr;
    OglTexture  *m_input1=nullptr;
    OglTexture  *m_input2=nullptr;
    OglTexture  *m_output=nullptr;
    int         m_RenderType;
    glm::mat2 matrix2x2={1.0f,0.0f,0.0f,1.0f}; 
    FrameBufferObject m_fbo;
};
#endif