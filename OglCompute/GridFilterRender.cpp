#include "GridFilterRender.hpp"


class ScaleShaderSSBO : public Shader
{
public:
    ScaleShaderSSBO()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                color = texture(tex0, texcoord);\n\
            }";

        m_cs =
#ifdef OpenGLES 
            "#version 320 es\n\
            precision highp float;\n\
            precision lowp  sampler2D; \n"
#else
            "#version 430\n"

#endif
            "uniform int nGridW;\
             uniform int srcW;\
             uniform int srcH;\
            layout(binding = 3) readonly buffer inputrdx{\
                float data[];\
            } uinputrdxBuffer;\
            layout(binding = 4) readonly buffer inputrdy{\
                float data[];\
            } uinputrdyBuffer;\
            layout(binding = 1) writeonly buffer OutNdx{\
                float data[];\
            } uOutNdxBuffer;\
            layout(binding = 2) writeonly buffer OutNdy{\
                float data[];\
            } uOutNdyBuffer;\
		     layout (local_size_x = 8, local_size_y = 8) in;\
		     void main() \
            {\
                 float mdx[2][2];\
                 float mdy[2][2];\
			     ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
                 int j =  storePos.y;\
                 int i = storePos.x;\
                 mdx[0][0] = uinputrdxBuffer.data[(j >> 5) * nGridW + (i >> 5)];\
                 mdy[0][0] = uinputrdyBuffer.data[(j >> 5) * nGridW + (i >> 5)];\
                 mdx[0][1] = uinputrdxBuffer.data[(j >> 5) * nGridW + (i >> 5) + 1];\
                 mdy[0][1] = uinputrdyBuffer.data[(j >> 5) * nGridW + (i >> 5) + 1];\
                 mdx[1][0] = uinputrdxBuffer.data[(j >> 5) * nGridW + (i >> 5) + nGridW];\
                 mdy[1][0] = uinputrdyBuffer.data[(j >> 5) * nGridW + (i >> 5) + nGridW];\
                 mdx[1][1] = uinputrdxBuffer.data[(j >> 5) * nGridW + (i >> 5) + nGridW + 1];\
                 mdy[1][1] = uinputrdyBuffer.data[(j >> 5) * nGridW + (i >> 5) + nGridW + 1]; \
                 float x_dif = ((1.0f * float(i) - float((i >> 5) << 5)) / 32.0f);\
                 float y_dif = ((1.0f * float(j) - float((j >> 5) << 5)) / 32.0f);\
                 float sdx = (1.0 - x_dif) * (1.0 - y_dif) * mdx[0][0] + (x_dif) * (1.0 - y_dif) * mdx[0][1] +\
                     (1.0 - x_dif) * (y_dif)*mdx[1][0] + (x_dif) * (y_dif)*mdx[1][1];\
                 float sdy = (1.0 - x_dif) * (1.0 - y_dif) * mdy[0][0] + (x_dif) * (1.0 - y_dif) * mdy[0][1] +\
                     (1.0 - x_dif) * (y_dif)*mdy[1][0] + (x_dif) * (y_dif)*mdy[1][1];\
                 int idx = int(sdx);\
                 int idy = int(sdy);\
                if (idx < 0 || idy < 0 || idx >= srcW || idy >= srcH)\
                    return; \
                 uOutNdxBuffer.data[idy * srcW + idx] = float(i);\
                 uOutNdyBuffer.data[idy * srcW + idx] = float(j);\
		     }";
    }
};
//precision highp float;
//if (idx < 0 || idy < 0 || idx >= srcW || idy >= srcH)
//    return; 
//vec4 tmp = (inputData * 255.0f - vec4(offset, offset, offset, offset)) * div_avg
//imageStore(tex0, storePos, tmp / 255.0f);
class ScaleShaderSwapRGB : public Shader
{
public:
    ScaleShaderSwapRGB()
    {
        m_fs = 
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float step_x = 1.0f/float(imgSize.x);\n\
                float step_y = 1.0f/float(imgSize.y);\n\
                vec4 color0 = texture(tex0, texcoord+vec2(0.0f,0.0f));\n\
                vec4 color1 = texture(tex0, texcoord+vec2(step_x,0));\n\
                vec4 color2 = texture(tex0, texcoord+vec2(0.0f,step_y));\n\
                vec4 color3 = texture(tex0, texcoord+vec2(step_x,step_y));\n\
                color = (color0+color1+color2+color3)/4.0f;\n\
            }";
    }
};

class ScaleShaderF32 : public Shader
{
public:
    ScaleShaderF32()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float step_x = 1.0f/float(imgSize.x);\n\
                float step_y = 1.0f/float(imgSize.y);\n\
                vec4 color0 = texture(tex0, texcoord+vec2(0.0f,0.0f)).rgba;\n\
                vec4 color1 = texture(tex0, texcoord+vec2(step_x,0).rgba;\n\
                vec4 color2 = texture(tex0, texcoord+vec2(0.0f,setp_y).rgba;\n\
                vec4 color3 = texture(tex0, texcoord+vec2(step_x,setp_y)).rgba;\n\
                color = (color0+color1+color2+color3)/4/0f;\n\
            }";
    }
};

GridFilterRender::GridFilterRender() 
{
    
}

GridFilterRender::~GridFilterRender()                
{
}

bool GridFilterRender::init()
{
   // initVao();
	checkGLError();
    checkGLError();
  //  printf(" m_Scaleyuv2RgbShader->compile sucessed\n");

    return true;
}

bool GridFilterRender::initUseCS()
{
    m_ScaleShaderSSBO2Rgb = new ScaleShaderSSBO();
    m_ScaleShaderSSBO2Rgb->compileCS();
    checkGLError();
    printf("compile sucessed\n");

    return true;
}

void GridFilterRender::updateParamer(int srcH, int srcW, int gridW,float *prdx,float *prdy)
{
    m_srcH = srcH;
    m_srcW = srcW;
    m_gridW = gridW;
    m_pRdx = prdx;
    m_pRdy = prdy;
}

void GridFilterRender::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void GridFilterRender::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void GridFilterRender::render()
{
	checkGLError();

}


void GridFilterRender::renderUseCS()
{
    CHECK;
    m_ssbo->bind();
    m_ssbo2->bind();
    m_inputssbo->bind();
    m_inpuitssbo2->bind();
    {
        m_ScaleShaderSSBO2Rgb->use();
        m_ScaleShaderSSBO2Rgb->setInt("nGridW", m_gridW); CHECK;
        m_ScaleShaderSSBO2Rgb->setInt("srcW", m_srcW); CHECK;
        m_ScaleShaderSSBO2Rgb->setInt("srcH", m_srcH); CHECK;
       // m_ScaleShaderSSBO2Rgb->setFloatArray("rdx", 256, m_pRdx);
       // m_ScaleShaderSSBO2Rgb->setFloatArray("rdy", 256, m_pRdy);
       // printf("m_output = [%d %d]", m_output->getWidth(), m_output->getHeight()); CHECK;
       // glBindImageTexture(0, m_output->getTextureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8); CHECK;
        glDispatchCompute(m_srcW / 8, m_srcH / 8, 1); CHECK;// 512^2 threads in blocks of 16^2
    }
    m_ssbo->unbind();
    m_ssbo2->unbind();
    m_inputssbo->unbind();
    m_inpuitssbo2->unbind();
}

void GridFilterRender::deinit()
{
    if (m_ScaleShader)
    {
        delete m_ScaleShader;
        m_ScaleShader = NULL;
    }

    if (m_ScaleShaderSSBO2Rgb)
    {
        delete m_ScaleShaderSSBO2Rgb;
        m_ScaleShaderSSBO2Rgb = nullptr;
    }
    deinitVao();
}
