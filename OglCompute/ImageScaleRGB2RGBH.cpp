#include "ImageScaleRGB2RGBH.hpp"

class ScaleShader : public Shader
{
public:
    ScaleShader()
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
            precision lowp  sampler2D;\n"
#else
            "#version 430\n"
#endif
            "layout(rgba8, binding = 0) readonly uniform image2D tex0; \
            layout(location = 2) uniform int uWidth;\
            layout(location = 3) uniform int uHeight;\
            layout(location = 4) uniform int uChannel;\
            layout(location = 5) uniform vec4 div_avg;\
            layout(location = 6) uniform vec4 offset;\
         layout(binding = 1) writeonly buffer InBuffer{\
             float data[];\
         } uInBuffer;\
		     layout (local_size_x = 8, local_size_y = 8) in;\
		     void main() {\
			 ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
             vec4 inputData = imageLoad(tex0,storePos);\
             vec4 tmp = (inputData * 255.0f - offset) * div_avg;\
             uInBuffer.data[storePos.y*uWidth*3+storePos.x*3+0] = tmp.x ;\
             uInBuffer.data[storePos.y*uWidth*3+storePos.x*3+1] = tmp.y ;\
             uInBuffer.data[storePos.y*uWidth*3+storePos.x*3+2] = tmp.z ;\
		 }";
    }
};
/*
short data[4];
for (int i = 0; i < 2; i++)
{
    for (int j = 0; j < 2; j++)
    {
        uInBuffer.data[(storePos.y*2 + i) * uWidth + (storePos.x*2 + j) + 0] = tmp.r + 2 * tmp.g + tmp.b;
    }
}
*/
class ScaleShaderSSBORgb2Rgbh : public Shader
{
public:
    ScaleShaderSSBORgb2Rgbh()
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
            "layout(rgba8, binding = 0) readonly uniform lowp image2D tex0; \
            layout(rgba8, binding = 1) writeonly uniform lowp image2D tex1;\
            layout(location = 2) uniform int uWidth;\
            layout(location = 3) uniform int uHeight;\
            layout(binding = 4) writeonly buffer InBuffer{\
             float data[];\
         } uInBuffer;\
		     layout (local_size_x = 8, local_size_y = 8) in;\
		     void main() {\
			 ivec2 storePos = ivec2(gl_GlobalInvocationID.xy)*2;\
             vec4 inputData0 = imageLoad(tex0,storePos);\
             vec4 inputData1 = imageLoad(tex0,storePos+ivec2(1,0));\
             vec4 inputData2 = imageLoad(tex0,storePos+ivec2(0,1));\
             vec4 inputData3 = imageLoad(tex0,storePos+ivec2(1,1));\
             vec4 tmp;\
             float Y[4];\
             Y[0] = (inputData0.r+2.0f*inputData0.g+inputData0.b);\
             Y[1] = (inputData1.r+2.0f*inputData1.g+inputData1.b);\
             Y[2] = (inputData2.r+2.0f*inputData2.g+inputData2.b);\
             Y[3] = (inputData3.r+2.0f*inputData3.g+inputData3.b);\
             uInBuffer.data[storePos.y*uWidth*2+storePos.x] = (3.0f*Y[0] - Y[1] - Y[2] - Y[3])/16.0;\
             uInBuffer.data[storePos.y*uWidth*2+storePos.x+1] =(3.0f*Y[1] -  Y[0] - Y[2] - Y[3])/16.0;\
             uInBuffer.data[storePos.y*uWidth*2+storePos.x+uWidth*2] = (3.0f*Y[2] -  Y[0] - Y[1] - Y[3])/16.0;\
             uInBuffer.data[storePos.y*uWidth*2+storePos.x+uWidth*2 +1] =(3.0f*Y[3] -  Y[0] - Y[1] - Y[2])/16.0;\
             imageStore(tex1, ivec2(gl_GlobalInvocationID.xy), (inputData0+inputData1+inputData2+inputData3)/4.0f);\
		 }";
    }
};

class ScaleShaderSSBORgbh2Rgb : public Shader
{
public:
    ScaleShaderSSBORgbh2Rgb()
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
            "layout(rgba8, binding = 0) readonly uniform lowp image2D tex0; \
            layout(rgba8, binding = 1) writeonly uniform lowp image2D tex1;\
            layout(location = 2) uniform int uWidth;\
            layout(location = 3) uniform int uHeight;\
            layout(binding = 4) readonly buffer InBuffer{\
             float data[];\
         } uInBuffer;\
		     layout (local_size_x = 8, local_size_y = 8) in;\
		     void main() {\
			 ivec2 storePos = ivec2(gl_GlobalInvocationID.xy)*2;\
             vec4 inputData0 = imageLoad(tex0,ivec2(gl_GlobalInvocationID.xy));\
             vec4 tmp;\
             vec4 Y[4];\
             Y[0].rgb = inputData0.rgb+vec3(uInBuffer.data[storePos.y*uWidth*2+storePos.x]);\
             Y[1].rgb = inputData0.rgb+vec3(uInBuffer.data[storePos.y*uWidth*2+storePos.x+1]);\
             Y[2].rgb = inputData0.rgb+vec3(uInBuffer.data[storePos.y*uWidth*2+storePos.x+uWidth*2]);\
             Y[3].rgb = inputData0.rgb+vec3(uInBuffer.data[storePos.y*uWidth*2+storePos.x+uWidth*2 +1]);\
             imageStore(tex1, storePos, Y[0]);\
             imageStore(tex1, storePos+ivec2(1,0), Y[1]);\
             imageStore(tex1, storePos+ivec2(0,1), Y[2]);\
             imageStore(tex1, storePos+ivec2(1,1), Y[3]);\
		 }";
    }
};

//precision highp float;

//vec4 tmp = (inputData * 255.0f - vec4(offset, offset, offset, offset)) * div_avg
//imageStore(tex0, storePos, tmp / 255.0f);


ImageRGB2GBHRender::ImageRGB2GBHRender()
{
    
}

ImageRGB2GBHRender::~ImageRGB2GBHRender()                
{
    deinit();
}

bool ImageRGB2GBHRender::init()
{
    initVao();
	checkGLError();
# if 0
    m_ScaleShader = new ScaleShader();
    m_ScaleShader->compile();
	checkGLError();
    printf("m_ScaleShader->compile\n");
    m_Scale2YuvShader = new Scale2YuvShader();
    m_Scale2YuvShader->compile();
    checkGLError();
    printf("m_Scale2YuvShader->compile()\n");
    m_Scaleyuv2RgbShader = new Yuv2RgbShader();
    m_Scaleyuv2RgbShader->compile();
	checkGLError();

    m_ScaleRotationShader = new ScaleRotationShader();
    m_ScaleRotationShader->compile();
	checkGLError();

    m_ScaleSwapRGBShader = new ScaleShaderSwapRGB();
    m_ScaleSwapRGBShader->compile();
    checkGLError();
    printf(" m_Scaleyuv2RgbShader->compile sucessed\n");
#endif
    return true;
}

bool ImageRGB2GBHRender::initUseCS()
{
 
    m_ScaleRgb2RgbhShader = new ScaleShaderSSBORgb2Rgbh();
    m_ScaleRgbh2RgbShader = new ScaleShaderSSBORgbh2Rgb();
    CHECK;
    m_ScaleRgb2RgbhShader->compileCS();
    m_ScaleRgbh2RgbShader->compileCS();
    checkGLError();
    printf("compile sucessed\n");
    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
      work_grp_size[0], work_grp_size[1], work_grp_size[2]);
    return true;
}

void ImageRGB2GBHRender::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void ImageRGB2GBHRender::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageRGB2GBHRender::render()
{
    // downscale
	//checkGLError();
    CHECK;
    m_fbo.attachTexture(m_output); CHECK;
    m_fbo.bind(); CHECK;
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight()); CHECK;
    glClearColor(0.0, 0.0, 0.0, 1.0); CHECK;
#if 0
    if (m_RenderType == E_ImageScale_RGB2YUV)
    {
        m_Scale2YuvShader->use();
        m_Scale2YuvShader->setInt("tex0", 0);
    }
    else if(m_RenderType == E_ImageScale_YUV2RGB)
    {
        m_Scaleyuv2RgbShader->use();
        m_Scaleyuv2RgbShader->setInt("tex0",0);
    }
    else if(m_RenderType == E_ImageScale_Rotation)
    {
        m_ScaleRotationShader->use();
        m_ScaleRotationShader->setInt("tex0",0);
        m_ScaleRotationShader->setMat2("glRotation",matrix2x2);
    }
    else if(m_RenderType == E_ImageScale_SWAPBGR)
    {
        m_ScaleSwapRGBShader->use();
        m_ScaleSwapRGBShader->setInt("tex0",0);
    }
    else
    {
        m_ScaleShader->use();
        m_ScaleShader->setInt("tex0", 0);
    }
#endif
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    m_input->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);
    
    glBindVertexArray(0);
    m_fbo.unbind();
    m_input->unbind();
   // glFinish();
	checkGLError();

}


void ImageRGB2GBHRender::renderUseCS()
{

    m_ssbo->bind();
    if (m_RenderType == E_ImageScale_RGBH2RGB)
    {
        m_ScaleRgbh2RgbShader->use(); CHECK;
        m_ScaleRgbh2RgbShader->setInt("uWidth", m_input->getWidth()); CHECK;
        m_ScaleRgbh2RgbShader->setInt("uHeight", m_input->getWidth()); CHECK;
        glBindImageTexture(0, m_input->getTextureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); CHECK;
        glBindImageTexture(1, m_output->getTextureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8); CHECK;
        printf("m_input = [%d %d]", m_input->getWidth(), m_input->getHeight()); CHECK;
        glDispatchCompute((m_input->getWidth()) / 8, (m_input->getHeight()) / 8, 1); // 512^2 threads in blocks of 16^2CHECK;
        CHECK;
    }
    else
    {
        m_ScaleRgb2RgbhShader->use(); CHECK;
        m_ScaleRgb2RgbhShader->setInt("uWidth", m_output->getWidth()); CHECK;
        m_ScaleRgb2RgbhShader->setInt("uHeight", m_output->getWidth()); CHECK;
        printf("m_output = [%d %d]", m_output->getWidth(), m_output->getHeight()); CHECK;
        glBindImageTexture(0, m_input->getTextureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); CHECK;
        glBindImageTexture(1, m_output->getTextureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8); CHECK;
        glDispatchCompute(m_output->getWidth() / 8, m_output->getHeight() / 8, 1); CHECK;// 512^2 threads in blocks of 16^2
    }
    m_ssbo->unbind();
}

void ImageRGB2GBHRender::deinit()
{
#if 0
    if (m_ScaleShader)
    {
        delete m_ScaleShader;
        m_ScaleShader = NULL;
    }
    if (m_Scale2YuvShader)
    {
        delete m_Scale2YuvShader;
        m_Scale2YuvShader = NULL;
    }
    if (m_Scaleyuv2RgbShader)
    {
        delete m_Scaleyuv2RgbShader;
        m_Scaleyuv2RgbShader = NULL;
    }
    if (m_ScaleSwapRGBShader)
    {
        delete m_ScaleSwapRGBShader;
        m_ScaleSwapRGBShader = NULL;
    }
    if (m_ScaleRotationShader)
    {
        delete m_ScaleRotationShader;
        m_ScaleRotationShader = NULL;
    }
#endif
    if (m_ScaleRgb2RgbhShader)
    {
        delete m_ScaleRgb2RgbhShader;
        m_ScaleRgb2RgbhShader = nullptr;
    }
    if (m_ScaleRgbh2RgbShader)
    {
        delete m_ScaleRgbh2RgbShader;
        m_ScaleRgbh2RgbShader = nullptr;
    }
    deinitVao();
}
