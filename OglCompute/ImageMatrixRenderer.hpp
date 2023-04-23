#ifndef __ImageMatrixRenderer_HPP__
#define __ImageMatrixRenderer_HPP__

#include "Renderer.hpp"
#include "FrameBufferObject.hpp"
#include <vector>
#include <stdlib.h>

class ImageMatrixRenderer : public Renderer
{
public:
    ImageMatrixRenderer();

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
    void setTabRTextureR(OglTexture *tex){
        m_inputR = tex;
    }
    void setTabRTextureG(OglTexture *tex){
        m_inputG = tex;
    }
    void setTabRTextureB(OglTexture *tex){
        m_inputB = tex;
    }
    void setMatrix3x3(float *matrix)
    {
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
               // matrix3x3[i+3+j] = matrix[i+3+j];
                matrix3x3[i][j] = matrix[i+3+j];
            }
        }
    }
    void updataMesh(int mesh_w, int mesh_h, int img_w, int img_h,int nDim,float *pH);
    void initVAO(int mesh_w, int mesh_h);
	virtual	void deinit();

private:
    Shader      *m_MatrixShader;
    OglTexture  *m_input;
    OglTexture  *m_inputR;
    OglTexture  *m_inputG;
    OglTexture  *m_inputB;
    glm::mat3 matrix3x3;
    OglTexture  *m_output;
    std::vector<float> m_pos;
    std::vector<unsigned int> m_indice;
    FrameBufferObject m_fbo;
};
#endif