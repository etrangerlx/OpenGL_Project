#include "ImageBlendRenderer.hpp"

class Conv3HShader : public Shader
{
public:
	Conv3HShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - xStep, texcoord.y))\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 2.0\n\
                        + texture(tex0, vec2(texcoord.x + xStep, texcoord.y));\n\
                color = sum / 4.0;\n\
            }";
	}
};

class Conv3VShader : public Shader
{
public:
	Conv3VShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x, texcoord.y - yStep))\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 2.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + yStep));\n\
                color = sum / 4.0;\n\
            }";
	}
};

class Conv5HShader : public Shader
{
public:
	Conv5HShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - 2.0 * xStep, texcoord.y))\n\
                        + texture(tex0, vec2(texcoord.x - xStep, texcoord.y)) * 4.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 6.0\n\
                        + texture(tex0, vec2(texcoord.x + xStep, texcoord.y)) * 4.0\n\
                        + texture(tex0, vec2(texcoord.x + 2.0 * xStep, texcoord.y));\n\
                color = sum / 16.0;\n\
            }";
	}
};

class Conv5VShader : public Shader
{
public:
	Conv5VShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x, texcoord.y - 2.0 * yStep))\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y - yStep)) * 4.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 6.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + yStep)) * 4.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + 2.0 * yStep));\n\
                color = sum / 16.0;\n\
            }";
	}
};

class Conv7HShader : public Shader
{
public:
	Conv7HShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - 3.0 * xStep, texcoord.y))\n\
                        + texture(tex0, vec2(texcoord.x - 2.0 * xStep, texcoord.y)) * 6.0\n\
                        + texture(tex0, vec2(texcoord.x - xStep, texcoord.y)) * 15.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 20.0\n\
                        + texture(tex0, vec2(texcoord.x + xStep, texcoord.y)) * 15.0\n\
                        + texture(tex0, vec2(texcoord.x + 2.0 * xStep, texcoord.y)) * 6.0\n\
                        + texture(tex0, vec2(texcoord.x + 3.0 * xStep, texcoord.y));\n\
                color = sum / 64.0;\n\
            }";
	}
};

class Conv7VShader : public Shader
{
public:
	Conv7VShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x, texcoord.y - 3.0 * yStep))\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y - 2.0 * yStep)) * 6.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y - yStep)) * 15.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 20.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + yStep)) * 15.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + 2.0 * yStep)) * 6.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + 3.0 * yStep));\n\
                color = sum / 64.0;\n\
            }";
	}
};

class Conv9HShader : public Shader
{
public:
	Conv9HShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - 4.0 * xStep, texcoord.y)) * 12.0\n\
                        + texture(tex0, vec2(texcoord.x - 3.0 * xStep, texcoord.y)) * 30.0\n\
                        + texture(tex0, vec2(texcoord.x - 2.0 * xStep, texcoord.y)) * 56.0\n\
                        + texture(tex0, vec2(texcoord.x - xStep, texcoord.y)) * 81.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 92.0\n\
                        + texture(tex0, vec2(texcoord.x + xStep, texcoord.y)) * 81.0\n\
                        + texture(tex0, vec2(texcoord.x + 2.0 * xStep, texcoord.y)) * 56.0\n\
                        + texture(tex0, vec2(texcoord.x + 3.0 * xStep, texcoord.y)) * 30.0\n\
                        + texture(tex0, vec2(texcoord.x + 4.0 * xStep, texcoord.y)) * 12.0;\n\
                color = sum / 450.0;\n\
            }";
	}
};

class Conv9VShader : public Shader
{
public:
	Conv9VShader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x, texcoord.y - 4.0 * yStep)) * 12.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y - 3.0 * yStep)) * 30.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y - 2.0 * yStep)) * 56.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y - yStep)) * 81.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 92.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + yStep)) * 81.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + 2.0 * yStep)) * 56.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + 3.0 * yStep)) * 30.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + 4.0 * yStep)) * 12.0;\n\
                color = sum / 450.0;\n\
            }";
	}
};

class Conv3x3Shader : public Shader
{
public:
	Conv3x3Shader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x, texcoord.y - yStep)) * 2.0\n\
                        + texture(tex0, vec2(texcoord.x - xStep, texcoord.y)) * 2.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 4.0\n\
                        + texture(tex0, vec2(texcoord.x + xStep, texcoord.y)) * 2.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y + yStep)) * 2.0;\n\
                color = sum / 12.0;\n\
            }";
	}
};

class Conv5x5Shader : public Shader
{
public:
	Conv5x5Shader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - 0.5 * xStep, texcoord.y - 1.5 * yStep)) * 50.0\n\
                        + texture(tex0, vec2(texcoord.x - 1.5 * xStep, texcoord.y + 0.5 * yStep)) * 50.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 36.0\n\
                        + texture(tex0, vec2(texcoord.x + 1.5 * xStep, texcoord.y - 0.5 * yStep)) * 50.0\n\
                        + texture(tex0, vec2(texcoord.x + 0.5 * xStep, texcoord.y + 1.5 * yStep)) * 50.0;\n\
                color = sum / 236.0;\n\
            }";
	}
};

class Conv7x7Shader : public Shader
{
public:
	Conv7x7Shader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - 2.5 * xStep, texcoord.y - 1.5 * yStep)) * 147.0\n\
                        + texture(tex0, vec2(texcoord.x - 0.5 * xStep, texcoord.y - 1.5 * yStep)) * 735.0\n\
                        + texture(tex0, vec2(texcoord.x + 1.5 * xStep, texcoord.y - 2.5 * yStep)) * 147.0\n\
                        + texture(tex0, vec2(texcoord.x - 1.5 * xStep, texcoord.y + 0.5 * yStep)) * 735.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 400.0\n\
                        + texture(tex0, vec2(texcoord.x + 1.5 * xStep, texcoord.y - 0.5 * yStep)) * 735.0\n\
                        + texture(tex0, vec2(texcoord.x - 1.5 * xStep, texcoord.y + 2.5 * yStep)) * 147.0\n\
                        + texture(tex0, vec2(texcoord.x + 0.5 * xStep, texcoord.y + 1.5 * yStep)) * 735.0\n\
                        + texture(tex0, vec2(texcoord.x + 2.5 * xStep, texcoord.y + 1.5 * yStep)) * 147.0;\n\
                color = sum / 3928.0;\n\
            }";
	}
};

class Conv9x9Shader : public Shader
{
public:
	Conv9x9Shader()
	{
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec4 sum = texture(tex0, vec2(texcoord.x - 2.5 * xStep, texcoord.y - 1.5 * yStep)) * 945.0\n\
                        + texture(tex0, vec2(texcoord.x - 0.5 * xStep, texcoord.y - 1.5 * yStep)) * 1912.0\n\
                        + texture(tex0, vec2(texcoord.x + 1.5 * xStep, texcoord.y - 2.5 * yStep)) * 945.0\n\
                        + texture(tex0, vec2(texcoord.x - 1.5 * xStep, texcoord.y + 0.5 * yStep)) * 1912.0\n\
                        + texture(tex0, vec2(texcoord.x, texcoord.y)) * 682.0\n\
                        + texture(tex0, vec2(texcoord.x + 1.5 * xStep, texcoord.y - 0.5 * yStep)) * 1912.0\n\
                        + texture(tex0, vec2(texcoord.x - 1.5 * xStep, texcoord.y + 2.5 * yStep)) * 945.0\n\
                        + texture(tex0, vec2(texcoord.x + 0.5 * xStep, texcoord.y + 1.5 * yStep)) * 1912.0\n\
                        + texture(tex0, vec2(texcoord.x + 2.5 * xStep, texcoord.y + 1.5 * yStep)) * 945.0\n\
                        + texture(tex0, vec2(texcoord.x - 0.5 * xStep, texcoord.y - 3.5 * yStep)) * 589.0\n\
                        + texture(tex0, vec2(texcoord.x - 3.5 * xStep, texcoord.y + 0.5 * yStep)) * 589.0\n\
                        + texture(tex0, vec2(texcoord.x + 3.5 * xStep, texcoord.y - 0.5 * yStep)) * 589.0\n\
                        + texture(tex0, vec2(texcoord.x + 0.5 * xStep, texcoord.y + 3.5 * yStep)) * 589.0;\n\
                color = sum / 14466.0;\n\
            }";
	}
};

class BilateralShader : public Shader
{
public:
    BilateralShader()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                ivec2 imgSize = textureSize(tex0,0);\n\
                float xStep = 1.0 / float(imgSize.x);\n\
                float yStep = 1.0 / float(imgSize.y);\n\
                vec3 row1, row2, row3, w1, w2, w3;\n\
                row1 = vec3(texture(tex0, vec2(texcoord.x - xStep, texcoord.y - yStep)).r * 2.0, \n\
                            texture(tex0, vec2(texcoord.x, texcoord.y - yStep)).r * 2.0, \n\
                            texture(tex0, vec2(texcoord.x + xStep, texcoord.y - yStep)).r * 2.0);\n\
                row2 = vec3(texture(tex0, vec2(texcoord.x - xStep, texcoord.y)).r * 2.0, \n\
                            texture(tex0, vec2(texcoord.x, texcoord.y)).r * 2.0, \n\
                            texture(tex0, vec2(texcoord.x + xStep, texcoord.y)).r * 2.0);\n\
                row3 = vec3(texture(tex0, vec2(texcoord.x - xStep, texcoord.y + yStep)).r * 2.0, \n\
                            texture(tex0, vec2(texcoord.x, texcoord.y + yStep)).r * 2.0, \n\
                            texture(tex0, vec2(texcoord.x + xStep, texcoord.y + yStep)).r * 2.0);\n\
                vec3 center = vec3(row2[1], row2[1], row2[1]);\n\
                w1 = row1 - center; w2 = row2 - center; w3 = row3 - center;\n\
                float mask;\n\
                // if (w1 == vec3(0.0, 0.0, 0.0) && w2 == vec3(0.0, 0.0, 0.0) && w3 == vec3(0.0, 0.0, 0.0)){\n\
                //     mask = row2[1];\n\
                // } else {\n\
                w1 = w1 * w1; w2 = w2 * w2; w3 = w3 * w3;\n\
                float sum = w1[0] + w1[1] + w1[2] + w2[0] + w2[1] + w2[2] + w3[0] + w3[1] + w3[2];\n\
                w1 = vec3(1.0, 1.0, 1.0) - clamp(w1 / sum, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)); \n\
                w2 = vec3(1.0, 1.0, 1.0) - clamp(w2 / sum, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)); \n\
                w3 = vec3(1.0, 1.0, 1.0) - clamp(w3 / sum, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));\n\
                center = row1 * w1 + row2 * w2 + row3 * w3;\n\
                    mask = (center[0] + center[1] + center[2]) / 9.0;\n\
                // }\n\
                color = vec4(mask, mask, mask, 1.0);\n\
            }";
    }
};

class BlendShader : public Shader
{
public:
    BlendShader()
    {
        m_fs =
            "uniform sampler2D tex0;\n\
            uniform sampler2D tex1;\n\
            uniform sampler2D tex2;\n\
            uniform vec2 maskTrans;\n\
            in vec2 texcoord;\n\
            out vec4 color;\n\
            void main() {\n\
                vec4 rgba0 = texture(tex0, texcoord);\n\
                vec4 rgba1 = texture(tex1, texcoord);\n\
                float mask = texture(tex2, texcoord + maskTrans).r;\n\
                color.rgb = mix(rgba1.rgb, rgba0.rgb, mask);\n\
                color.a = 1.0;\n\
            }";
    }
};

ImageBlendRenderer::ImageBlendRenderer() 
                : m_blendShader(nullptr)
				, m_conv3HShader(nullptr)
				, m_conv3VShader(nullptr)
                , m_conv5HShader(nullptr)
                , m_conv5VShader(nullptr)
                , m_conv7HShader(nullptr)
                , m_conv7VShader(nullptr)
                , m_conv9HShader(nullptr)
                , m_conv9VShader(nullptr)
                , m_bilateralShader(nullptr)
                , m_conv3x3OptShader(nullptr)
                , m_conv5x5OptShader(nullptr)
                , m_conv7x7OptShader(nullptr)
                , m_conv9x9OptShader(nullptr)
                , m_RenderType(E_ImageBlend)
{
    
}

bool ImageBlendRenderer::init()
{
    initVao();

    m_blendShader = new BlendShader();
    m_blendShader->compile();

    m_bilateralShader = new BilateralShader();
    m_bilateralShader->compile();

    m_conv3HShader = new Conv3HShader();
    m_conv3HShader->compile();

    m_conv3VShader = new Conv3VShader();
    m_conv3VShader->compile();

    m_conv5HShader = new Conv5HShader();
    m_conv5HShader->compile();

    m_conv5VShader = new Conv5VShader();
    m_conv5VShader->compile();

    m_conv7HShader = new Conv7HShader();
    m_conv7HShader->compile();

    m_conv7VShader = new Conv7VShader();
    m_conv7VShader->compile();

    m_conv9HShader = new Conv9HShader();
    m_conv9HShader->compile();

    m_conv9VShader = new Conv9VShader();
    m_conv9VShader->compile();

    m_conv3x3OptShader = new Conv3x3Shader();
    m_conv3x3OptShader->compile();

    m_conv5x5OptShader = new Conv5x5Shader();
    m_conv5x5OptShader->compile();

    m_conv7x7OptShader = new Conv7x7Shader();
    m_conv7x7OptShader->compile();

    m_conv9x9OptShader = new Conv9x9Shader();
    m_conv9x9OptShader->compile();

    m_maskTrans = glm::vec2(0.f, 0.f);

    return true;
}

void ImageBlendRenderer::setInput0Texture(OglTexture *tex)
{
    m_input0 = tex;
}

void ImageBlendRenderer::setInput1Texture(OglTexture *tex)
{
    m_input1 = tex;
}

void ImageBlendRenderer::setInput2Texture(OglTexture *tex)
{
    m_input2 = tex;
}

void ImageBlendRenderer::setOutputTexture(OglTexture *tex)
{
    m_output = tex;
}

void ImageBlendRenderer::setMaskTrans(float dx, float dy)
{
    m_maskTrans = glm::vec2(dx, dy);
}

void ImageBlendRenderer::render()
{
    m_fbo.attachTexture(m_output);
    m_fbo.bind();
    glViewport(0, 0, m_output->getWidth(), m_output->getHeight());
    glClearColor(0.0, 0.0, 0.0, 1.0);

    Shader *useShader = nullptr;
    if(m_RenderType == E_ImageBlend)
        useShader = m_blendShader;
    else if (m_RenderType == E_Bilateral)
        useShader = m_bilateralShader;
    else if (m_RenderType == E_Conv3H)
        useShader = m_conv3HShader;
    else if (m_RenderType == E_Conv3V)
        useShader = m_conv3VShader;
    else if (m_RenderType == E_Conv5H)
        useShader = m_conv5HShader;
    else if (m_RenderType == E_Conv5V)
        useShader = m_conv5VShader;
    else if (m_RenderType == E_Conv7H)
        useShader = m_conv7HShader;
    else if (m_RenderType == E_Conv7V)
        useShader = m_conv7VShader;
    else if (m_RenderType == E_Conv9H)
        useShader = m_conv9HShader;
    else if (m_RenderType == E_Conv9V)
        useShader = m_conv9VShader;
    else if (m_RenderType == E_Conv3x3Opt)
        useShader = m_conv3x3OptShader;
    else if (m_RenderType == E_Conv5x5Opt)
        useShader = m_conv5x5OptShader;
    else if (m_RenderType == E_Conv7x7Opt)
        useShader = m_conv7x7OptShader;
    else if (m_RenderType == E_Conv9x9Opt)
        useShader = m_conv9x9OptShader;

    if (useShader == nullptr)
        return;

    useShader->use();
    glBindVertexArray(m_vao);

    glActiveTexture(GL_TEXTURE0);
    useShader->setInt("tex0", 0);
    m_input0->bind();

    if (m_RenderType == E_ImageBlend)
    {
        useShader->setVec2("maskTrans", m_maskTrans);
        glActiveTexture(GL_TEXTURE1);
        useShader->setInt("tex1", 1);
        m_input1->bind();

        glActiveTexture(GL_TEXTURE2);
        useShader->setInt("tex2", 2);
        m_input2->bind();
    } 

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);
    glBindVertexArray(0);
    m_fbo.unbind();
	
   // glFinish();
}

void ImageBlendRenderer::deinit()
{
    if (m_blendShader)
    {
        delete m_blendShader;
        m_blendShader = NULL;
    }
	
    if (m_bilateralShader)
    {
        delete m_bilateralShader;
        m_bilateralShader = NULL;
    }

    if (m_conv3HShader != NULL)
	{
		delete m_conv3HShader;
		m_conv3HShader = NULL;
	}
	
    if (m_conv3VShader != NULL)
	{
		delete m_conv3VShader;
		m_conv3VShader = NULL;
	}

    if (m_conv5HShader != NULL)
	{
		delete m_conv5HShader;
		m_conv5HShader = NULL;
	}

    if (m_conv5VShader != NULL)
	{
		delete m_conv5VShader;
		m_conv5VShader = NULL;
	}

    if (m_conv7HShader != NULL)
	{
		delete m_conv7HShader;
		m_conv7HShader = NULL;
	}

    if (m_conv7VShader != NULL)
	{
		delete m_conv7VShader;
		m_conv7VShader = NULL;
	}

    if (m_conv9HShader != NULL)
	{
		delete m_conv9HShader;
		m_conv9HShader = NULL;
	}

    if (m_conv9VShader != NULL)
	{
		delete m_conv9VShader;
		m_conv9VShader = NULL;
	}
	
    if (m_conv3x3OptShader != NULL)
	{
		delete m_conv3x3OptShader;
		m_conv3x3OptShader = NULL;
	}

    if (m_conv5x5OptShader != NULL)
	{
		delete m_conv5x5OptShader;
		m_conv5x5OptShader = NULL;
	}

    if (m_conv7x7OptShader != NULL)
	{
		delete m_conv7x7OptShader;
		m_conv7x7OptShader = NULL;
	}

    if (m_conv9x9OptShader != NULL)
	{
		delete m_conv9x9OptShader;
		m_conv9x9OptShader = NULL;
	}
	
    deinitVao();
}
