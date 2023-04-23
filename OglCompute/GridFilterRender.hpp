#ifndef __GridFilterRender_HPP__
#define __GridFilterRender_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"
#include "OglBaseMem.hpp"
//#define DEBUG__PRINT
#if defined(DEBUG__PRINT)
#define PRINT {\
    printf("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
    LOGI("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
}
#else
#define PRINT 
#endif

class GridFilterRender : public Renderer
{
public:
    GridFilterRender();
    ~GridFilterRender();
    void readTex(OglTexture* tex, char* OutBuf)
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
    void setInputTexture(OglTexture* tex);
    void updateParamer(int srcH, int srcW, int gridW, float* prdx, float* prdy);
    void setOutputTexture(OglTexture* tex);
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
    void setOutputSSBO1(OglBaseMem* ssbo) { m_ssbo2 = ssbo; }
    void setOutputSSBO2(OglBaseMem* ssbo) { m_ssbodx = ssbo; }
    void setOutputSSBO3(OglBaseMem* ssbo) { m_ssbody = ssbo; }
    void setInputSSBO(OglBaseMem* ssbo) { m_inputssbo = ssbo; }
    void setInputSSBO1(OglBaseMem* ssbo) { m_inpuitssbo2 = ssbo; }
    void setRenderType(int type)
    {
        m_RenderType = type;
    }

    virtual	void deinit();

private:
    Shader* m_ScaleShader;
    Shader* m_ScaleShaderSSBO2Rgb = nullptr;
    OglTexture* m_input;
    OglTexture* m_output;
    OglBaseMem* m_ssbo = nullptr;
    OglBaseMem* m_ssbo2 = nullptr;
    OglBaseMem* m_ssbodx = nullptr;
    OglBaseMem* m_ssbody = nullptr;
    OglBaseMem* m_inputssbo = nullptr;
    OglBaseMem* m_inpuitssbo2 = nullptr;
    int         m_RenderType;
    glm::mat2 matrix2x2 = { 1.0f,0.0f,0.0f,1.0f };
    FrameBufferObject m_fbo;
    int m_srcW = 0, m_srcH = 0, m_gridW = 0;
    float* m_pRdx = nullptr;
    float* m_pRdy = nullptr;
};

#endif