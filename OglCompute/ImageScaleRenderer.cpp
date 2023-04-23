#include "ImageScaleRenderer.hpp"

class ScaleShader : public Shader
{
public:
    ScaleShader()
    {
        m_vs =
#ifdef _WIN32
			"#version 430\n"
#else
			"#version 320 es\n\
             precision lowp float;\n"
#endif
			"layout(location = 0) in lowp vec3 in_vertex;\n\
				layout(location = 1) in lowp vec2 in_texcoord;\n\
				out lowp vec2 texcoord;\n\
				void main(){\n\
					gl_Position = vec4(in_vertex, 1.0);\n\
					texcoord = in_texcoord;\n\
				}";
        m_fs = 
            "#extension GL_EXT_YUV_target : require\n"
			"uniform samplerExternalOES tex0;\n\
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
            void main() {\n\
                color = texture(tex0, texcoord);\n\
            }";

        m_cs =
#ifdef OpenGLES 
            "#version 320 es\n\
            precision lowp float;\n\
            precision lowp  sampler2D;\n"
#else
            "#version 430\n"
#endif
            "layout(rgba8, binding = 0) readonly uniform lowp image2D tex0; \
            layout(location = 2) uniform int uWidth;\
            layout(location = 3) uniform int uHeight;\
            layout(location = 4) uniform int uChannel;\
            layout(location = 5) uniform vec4 div_avg;\
            layout(location = 6) uniform vec4 offset;\
         layout(binding = 1) writeonly buffer InBuffer{\
             float data[];\
         } uInBuffer;\
		     layout (local_size_x = 16, local_size_y = 16) in;\
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


class ScaleShaderSSBO : public Shader
{
public:
    ScaleShaderSSBO()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
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
            "layout(rgba8, binding = 0) writeonly uniform lowp image2D tex0; \
            layout(location = 2) uniform int uWidth;\
            layout(location = 3) uniform int uHeight;\
         layout(binding = 1) readonly buffer InBuffer{\
             float data[];\
         } uInBuffer;\
		     layout (local_size_x = 16, local_size_y = 16) in;\
		     void main() {\
			 ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
             vec4 tmp;\
             tmp.r = uInBuffer.data[storePos.y*uWidth*3+storePos.x*3+0] ;\
             tmp.g = uInBuffer.data[storePos.y*uWidth*3+storePos.x*3+1] ;\
             tmp.b = uInBuffer.data[storePos.y*uWidth*3+storePos.x*3+2] ;\
             tmp.a = 255.0f;\
             imageStore(tex0, storePos, tmp/255.0f);\
		 }";
    }
};

class ScaleShaderSwapRGB : public Shader
{
public:
    ScaleShaderSwapRGB()
    {
        m_fs = 
            "uniform sampler2D tex0;\n\
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
            precision lowp float;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                lowp float step_x = 1.0f/float(imgSize.x);\n\
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
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
            precision lowp float;\n\
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
//Y = 0.299*R + 0.587*G + 0.114*B;
//U = (B - Y) / 1.772 + 0.5;
//V = (R - Y) / 1.402 + 0.5;  

class Scale2YuvShader : public Shader
{
public:
    Scale2YuvShader()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
            lowp vec3 rgb2yuv(lowp vec3 rgb)    \n\
            {\n\
                lowp vec3 yuv; \n\
                yuv.x = 0.299 * rgb.x + 0.587 * rgb.y + 0.114 * rgb.z; \n\
                yuv.y = (rgb.z - yuv.x)/1.772 + 0.5f; \n\
                yuv.z = (rgb.x - yuv.x)/1.402 + 0.5f; \n\
                return yuv; \n\
            }\n\
            void main() {\n\
                color = texture(tex0, texcoord);\n\
                color.rgb = rgb2yuv(color.rgb);\n\
            }";
    }
};

class ScaleRotationShader : public Shader
{
public:
    ScaleRotationShader()
    {
        #if defined(ANDROID) && (__ANDROID_API__ >= 29)
        m_fs =
            "uniform samplerExternalOES tex0;\n\
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
            void main() {\n\
                color = texture(tex0, texcoord);\n\
            }";
        #else
        m_fs =
            "uniform sampler2D tex0;\n\
            in lowp vec2 texcoord;\n\
            out lowp vec4 color;\n\
            void main() {\n\
                color = texture(tex0, texcoord);\n\
            }";
        #endif
        m_vs = 
        #ifdef OpenGLES 
            "#version 320 es\n\
            #extension GL_OES_EGL_image_external_essl3 : enable \n\
            precision lowp float;\n"
        #else
            "#version 330\n"
        #endif
            "layout(location = 0) in vec3 in_vertex;\n\
            layout(location = 1) in vec2 in_texcoord;\n\
            uniform mat2 glRotation;\n\
            out lowp vec2 texcoord;\n\
            void main(){\n\
                gl_Position = vec4(glRotation*in_vertex.xy,in_vertex.z,1);\n\
                texcoord = in_texcoord;\n\
            }";    
    }
};

ImageScaleMeshRenderer::ImageScaleMeshRenderer() 
                : m_ScaleShader(nullptr)
                , m_RenderType(E_ImageScale)
{
    
}

ImageScaleMeshRenderer::~ImageScaleMeshRenderer()                
{
    deinit();
}

bool ImageScaleMeshRenderer::init()
{
    initVao();
	checkGLError();
    m_ScaleShader = new ScaleShader();
    m_ScaleShader->compile();

    return true;
}

bool ImageScaleMeshRenderer::initUseCS()
{
    m_ScaleShaderRgb2SSBO = new ScaleShader();
    m_ScaleShaderSSBO2Rgb = new ScaleShaderSSBO();
    m_ScaleShaderRgb2SSBO->compileCS();
    m_ScaleShaderSSBO2Rgb->compileCS();
    checkGLError();
    printf("compile sucessed\n");

    return true;
}


void ImageScaleMeshRenderer::initVAO2(int mesh_w, int mesh_h)
{
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);CHECK;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ImageScaleMeshRenderer::initMash(int mesh_w, int mesh_h)
{
    initVAO2(mesh_w, mesh_h);
    if (m_ScaleShader == NULL)
    {
        m_ScaleShader = new ScaleShader();
        m_ScaleShader->compile();
    }
    return ;

}

void ImageScaleMeshRenderer::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void ImageScaleMeshRenderer::setInputTexture1(OglTexture *tex)
{
    m_input1 = tex;
}

void ImageScaleMeshRenderer::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageScaleMeshRenderer::updataMesh(float* x_mesh, float* y_mesh, int mesh_w, int mesh_h, int mesh_grid_interval, int img_w, int img_h, int out_w, int out_h,int hr,int hc,int cy_hr,int cy_hc,float focal
    ,int startx,int starty,int roi_w,int roi_h)
{
    m_pos.clear();
    m_indice.clear();
    for (int h = 0; h < mesh_h; h++)
    {
        for (int w = 0; w < mesh_w; w++)
        {
            {
                float pos_x = (float)w * mesh_grid_interval / out_w * 2 - 1;
                pos_x = pos_x > 1.0 ? 1.0f : pos_x;
                float pos_y = (float)h * mesh_grid_interval / out_h * 2 - 1;
                pos_y = pos_y > 1.0 ? 1.0f : pos_y;
                float pos_z = 0;

                float theta = asin((y_mesh[h * mesh_w + w] - cy_hr) / focal);
                float fy = focal * tan(theta) + hr;
                float fx = (x_mesh[h * mesh_w + w] - cy_hc) / cos(theta) + hc;
                float pos_datax = fx;
                float pos_datay = fy;
                float tex_x = pos_datax / img_w;
                float tex_y = pos_datay / img_h;
                if (tex_x < 0)tex_x = 0;
                if (tex_x > 1)tex_x = 1;
                if (tex_y < 0)tex_y = 0;
                if (tex_y > 1)tex_y = 1;

                m_pos.push_back(pos_x);
                m_pos.push_back(pos_y);
                m_pos.push_back(pos_z);
                m_pos.push_back(tex_x);
                m_pos.push_back(tex_y);
            }
            if (h < mesh_h - 1 && w < mesh_w - 1)
            {
                m_indice.push_back(h * mesh_w + w);//��������index,����opengl��������
                m_indice.push_back(h * mesh_w + w + 1);
                m_indice.push_back((h + 1) * mesh_w + w + 1);
                m_indice.push_back(h * mesh_w + w);
                m_indice.push_back((h + 1) * mesh_w + w);
                m_indice.push_back((h + 1) * mesh_w + w + 1);
            }
        }
    }
    glBindVertexArray(m_vao);CHECK;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); CHECK;
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_pos.size(),&m_pos[0], GL_STATIC_DRAW); CHECK;
    checkGLError();
    glEnableVertexAttribArray(0); CHECK;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); CHECK;
    checkGLError();
    glEnableVertexAttribArray(1); CHECK;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); CHECK;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indice.size() * sizeof(unsigned int), &m_indice[0],
        GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkGLError();
}


void ImageScaleMeshRenderer::updataMeshUV(float* x_mesh, float* y_mesh, int mesh_w, int mesh_h, int mesh_grid_interval, int img_w, int img_h, int out_w, int out_h,int hr,int hc,int cy_hr,int cy_hc,float focal
    ,int startx,int starty,int roi_w,int roi_h)
{
    m_pos.clear();
    m_indice.clear();
    for (int h = 0; h < mesh_h; h++)
    {
        for (int w = 0; w < mesh_w; w++)
        {
            {
                float pos_x = (float)w * mesh_grid_interval / out_w * 2 - 1;
                pos_x = pos_x > 1.0 ? 1.0f : pos_x;
                float pos_y = (float)h * mesh_grid_interval / out_h * 2 - 1;
                pos_y = pos_y > 1.0 ? 1.0f : pos_y;
                float pos_z = 0;

                float theta = asin((y_mesh[h * mesh_w + w] - cy_hr) / focal);
                float fy = focal * tan(theta) + hr;
                float fx = (x_mesh[h * mesh_w + w] - cy_hc) / cos(theta) + hc;
                float pos_datax = fx;
                float pos_datay = fy;
                float tex_x = pos_datax / img_w;
                float tex_y = pos_datay / img_h;
                if (tex_x < 0)tex_x = 0;
                if (tex_x > 1)tex_x = 1;
                if (tex_y < 0)tex_y = 0;
                if (tex_y > 1)tex_y = 1;

                m_pos.push_back(pos_x);
                m_pos.push_back(pos_y);
                m_pos.push_back(pos_z);
                m_pos.push_back(tex_x);
                m_pos.push_back(tex_y);
            }
            if (h < mesh_h - 1 && w < mesh_w - 1)
            {
                m_indice.push_back(h * mesh_w + w);//��������index,����opengl��������
                m_indice.push_back(h * mesh_w + w + 1);
                m_indice.push_back((h + 1) * mesh_w + w + 1);
                m_indice.push_back(h * mesh_w + w);
                m_indice.push_back((h + 1) * mesh_w + w);
                m_indice.push_back((h + 1) * mesh_w + w + 1);
            }
        }
    }
    glBindVertexArray(m_vao);CHECK;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); CHECK;
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_pos.size(),&m_pos[0], GL_STATIC_DRAW); CHECK;
    checkGLError();
    glEnableVertexAttribArray(0); CHECK;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); CHECK;
    checkGLError();
    glEnableVertexAttribArray(1); CHECK;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); CHECK;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indice.size() * sizeof(unsigned int), &m_indice[0],
        GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    checkGLError();
}

void ImageScaleMeshRenderer::render()
{
    CHECK;
    m_fbo.attachTexture(m_output); CHECK;
    m_fbo.bind(); CHECK;
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight()); CHECK;
    glClearColor(0.0, 0.0, 0.0, 1.0); CHECK;
    {
        m_ScaleShader->use();
        m_ScaleShader->setInt("tex0", 0);
        glActiveTexture(GL_TEXTURE0);CHECK;
    }
    m_input->bind();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indice.size(), GL_UNSIGNED_INT, (GLvoid*)0);CHECK;
    glBindVertexArray(0);
    m_fbo.unbind();
    m_input->unbind();
	checkGLError();

}


void ImageScaleMeshRenderer::renderUseCS()
{
    CHECK;
    m_ssbo->bind();
   
        m_ScaleShaderRgb2SSBO->use();
        m_ScaleShaderRgb2SSBO->setInt("uWidth", m_input->getWidth()); CHECK;
        m_ScaleShaderRgb2SSBO->setInt("uHeight", m_input->getWidth()); CHECK;
        m_ScaleShaderRgb2SSBO->setInt("uChannel", 3); CHECK;
        m_ScaleShaderRgb2SSBO->setVec4("offset", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)); CHECK;//b=(a-offset)*div_avg
        m_ScaleShaderRgb2SSBO->setVec4("div_avg", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); CHECK;
        printf("m_input = [%d %d]", m_input->getWidth(), m_input->getHeight()); CHECK;
        glBindImageTexture(0, m_input->getTextureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); CHECK;
        glDispatchCompute(m_input->getWidth() / 16, m_input->getHeight() / 16, 1); // 512^2 threads in blocks of 16^2CHECK;
  //  }
    /*
    {
        m_ScaleShaderSSBO2Rgb->use();
        m_ScaleShaderSSBO2Rgb->setInt("uWidth", m_output->getWidth()); CHECK;
        m_ScaleShaderSSBO2Rgb->setInt("uHeight", m_output->getWidth()); CHECK;
        printf("m_output = [%d %d]", m_output->getWidth(), m_output->getHeight()); CHECK;
        glBindImageTexture(0, m_output->getTextureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8); CHECK;
        glDispatchCompute(m_output->getWidth() / 16, m_output->getHeight() / 16, 1); CHECK;// 512^2 threads in blocks of 16^2
    }
    */
    m_ssbo->unbind();
}

void ImageScaleMeshRenderer::deinit()
{
    if (m_ScaleShader)
    {
        delete m_ScaleShader;
        m_ScaleShader = NULL;
    }
    if(m_ScaleShaderRgb2SSBO)
    {
        delete m_ScaleShaderRgb2SSBO;
        m_ScaleShaderRgb2SSBO = nullptr;
    }
    if (m_ScaleShaderSSBO2Rgb)
    {
        delete m_ScaleShaderSSBO2Rgb;
        m_ScaleShaderSSBO2Rgb = nullptr;
    }
    deinitVao();
}

ImageScaleRenderer::ImageScaleRenderer() 
                : m_ScaleShader(nullptr)
                , m_Scale2YuvShader(nullptr)
                , m_Scaleyuv2RgbShader(nullptr)
                , m_ScaleRotationShader(nullptr)
                , m_ScaleSwapRGBShader(nullptr)
                , m_RenderType(E_ImageScale)
{
    
}

ImageScaleRenderer::~ImageScaleRenderer()                
{
    deinit();
}

bool ImageScaleRenderer::init()
{
    initVao();
	checkGLError();

    m_ScaleShader = new ScaleShader();
    m_ScaleShader->compile();
	checkGLError();
    printf("m_ScaleShader->compile\n");
    m_Scale2YuvShader = new Scale2YuvShader();
    m_Scale2YuvShader->compile();
    checkGLError();
    printf("m_Scale2YuvShader->compile()\n");
   // m_Scaleyuv2RgbShader = new Yuv2RgbShader();
   // m_Scaleyuv2RgbShader->compile();
	checkGLError();

    m_ScaleRotationShader = new ScaleRotationShader();
    m_ScaleRotationShader->compile();
	checkGLError();
    printf("m_ScaleRotationShader->compile\n");
    m_ScaleSwapRGBShader = new ScaleShaderSwapRGB();
    m_ScaleSwapRGBShader->compile();
    checkGLError();
    printf(" m_Scaleyuv2RgbShader->compile sucessed\n");

    return true;
}

bool ImageScaleRenderer::initUseCS()
{
    m_ScaleShaderRgb2SSBO = new ScaleShader();
    m_ScaleShaderSSBO2Rgb = new ScaleShaderSSBO();
    m_ScaleShaderRgb2SSBO->compileCS();
    m_ScaleShaderSSBO2Rgb->compileCS();
    checkGLError();
    printf("compile sucessed\n");

    return true;
}

void ImageScaleRenderer::setInputTexture(OglTexture *tex)
{
    m_input = tex;
}

void ImageScaleRenderer::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageScaleRenderer::render()
{
    // downscale
	//checkGLError();
    CHECK;
    m_fbo.attachTexture(m_output); CHECK;
    m_fbo.bind(); CHECK;
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight()); CHECK;
    glClearColor(0.0, 0.0, 0.0, 1.0); CHECK;

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


void ImageScaleRenderer::renderUseCS()
{
    CHECK;
    m_ssbo->bind();
    if (m_RenderType != E_SSBO2Texture)
    {
        m_ScaleShaderRgb2SSBO->use();
        m_ScaleShaderRgb2SSBO->setInt("uWidth", m_input->getWidth()); CHECK;
        m_ScaleShaderRgb2SSBO->setInt("uHeight", m_input->getWidth()); CHECK;
        m_ScaleShaderRgb2SSBO->setInt("uChannel", 3); CHECK;
        m_ScaleShaderRgb2SSBO->setVec4("offset", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)); CHECK;//b=(a-offset)*div_avg
        m_ScaleShaderRgb2SSBO->setVec4("div_avg", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); CHECK;
        printf("m_input = [%d %d]", m_input->getWidth(), m_input->getHeight()); CHECK;
        glBindImageTexture(0, m_input->getTextureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); CHECK;
        glDispatchCompute(m_input->getWidth() / 16, m_input->getHeight() / 16, 1); // 512^2 threads in blocks of 16^2CHECK;
    }
    else
    {
        m_ScaleShaderSSBO2Rgb->use();
        m_ScaleShaderSSBO2Rgb->setInt("uWidth", m_output->getWidth()); CHECK;
        m_ScaleShaderSSBO2Rgb->setInt("uHeight", m_output->getWidth()); CHECK;
        printf("m_output = [%d %d]", m_output->getWidth(), m_output->getHeight()); CHECK;
        glBindImageTexture(0, m_output->getTextureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8); CHECK;
        glDispatchCompute(m_output->getWidth() / 16, m_output->getHeight() / 16, 1); CHECK;// 512^2 threads in blocks of 16^2
    }
    m_ssbo->unbind();
}

void ImageScaleRenderer::deinit()
{
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
    if(m_ScaleShaderRgb2SSBO)
    {
        delete m_ScaleShaderRgb2SSBO;
        m_ScaleShaderRgb2SSBO = nullptr;
    }
    if (m_ScaleShaderSSBO2Rgb)
    {
        delete m_ScaleShaderSSBO2Rgb;
        m_ScaleShaderSSBO2Rgb = nullptr;
    }
    deinitVao();
}