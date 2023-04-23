#ifndef __ImageScaleRenderer_HPP__
#define __ImageScaleRenderer_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"
#include "OglBaseMem.hpp"
////#define DEBUG__PRINT
//#if defined(DEBUG__PRINT)
//#define PRINT {\
//    printf("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
//    LOGI("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
//}
//#else
//#define PRINT
//#endif

enum IMAGESCALE_FORMAT
{
    E_ImageScale = 0,
    E_ImageScale_RGB2YUV = 1,
    E_ImageScale_YUV2RGB = 2,
    E_ImageScale_Rotation = 3,
    E_ImageScale_SWAPBGR = 4,
    E_SSBO2Texture = 5,
    E_Texture2SSBO
};

class ImageScaleMeshRenderer : public Renderer
{
public:
    ImageScaleMeshRenderer();
    ~ImageScaleMeshRenderer();
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
    void setInputTexture1(OglTexture *tex);
    void initMash(int mesh_w, int mesh_h);
    void initVAO2(int mesh_w, int mesh_h);
    void updataMesh(float* x_mesh, float* y_mesh, int mesh_w, int mesh_h, int mesh_grid_interval, int img_w, int img_h, int out_w, int out_h, int hr, int hc, int cy_hr, int cy_hc, float focal,
         int startx, int starty, int roi_w, int roi_h);
    void updataMeshUV(float* x_mesh, float* y_mesh, int mesh_w, int mesh_h, int mesh_grid_interval, int img_w, int img_h, int out_w, int out_h, int hr, int hc, int cy_hr, int cy_hc, float focal,
         int startx, int starty, int roi_w, int roi_h);
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
    Shader      *m_ScaleShader;
    Shader* m_ScaleShaderRgb2SSBO=nullptr;
    Shader* m_ScaleShaderSSBO2Rgb = nullptr;
    OglTexture  *m_input;
    OglTexture  *m_input1;
    OglTexture  *m_output;
    OglBaseMem* m_ssbo = nullptr;
    int         m_RenderType;
    glm::mat2 matrix2x2={1.0f,0.0f,0.0f,1.0f}; 
    std::vector<float> m_pos;
    std::vector<unsigned int> m_indice;
    FrameBufferObject m_fbo;
};


class ImageScaleRenderer : public Renderer
{
public:
    ImageScaleRenderer();
    ~ImageScaleRenderer();
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
    Shader      *m_ScaleShader;
    Shader* m_ScaleShaderRgb2SSBO=nullptr;
    Shader* m_ScaleShaderSSBO2Rgb = nullptr;
    Shader      *m_Scale2YuvShader;
    Shader      *m_Scaleyuv2RgbShader;
    Shader      *m_ScaleRotationShader; 
    Shader      *m_ScaleSwapRGBShader;
    OglTexture  *m_input;
    OglTexture  *m_output;
    OglBaseMem* m_ssbo = nullptr;
    int         m_RenderType;
    glm::mat2 matrix2x2={1.0f,0.0f,0.0f,1.0f}; 
    FrameBufferObject m_fbo;
};
#endif