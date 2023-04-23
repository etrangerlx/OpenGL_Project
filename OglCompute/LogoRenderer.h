#ifndef __LOGO_RENDERER_H__
#define __LOGO_RENDERER_H__

#include "../OglCompute/Renderer.hpp"
#include "../OglCompute/FrameBufferObject.hpp"

class LogoShader : public Shader
{
public:
	LogoShader()
	{
		m_vs =
#ifdef _WIN32
			"#version 330\n"
#else
			"#version 320 es\n\
                precision highp float;\n"
#endif
			"layout(location = 0) in vec3 in_vertex;\n\
				layout(location = 1) in vec3 in_texcoord;\n\
				uniform mat4 MVP;\n\
				out vec3 texcoord;\n\
				void main(){\n\
					gl_Position = MVP * vec4(in_vertex, 1.f);\n\
					texcoord = in_texcoord;\n\
				}";

		m_fs =
			"in vec3 texcoord;\n\
				uniform sampler2D tex;\n\
				out vec4 color;\n\
				void main() {\n\
					color = texture(tex, texcoord.xy);\n\
					color.a = color.r;\n\
				}";
	}
};


class LogoRenderer : public Renderer
{
public:
	LogoRenderer();
	~LogoRenderer();
	void setInputTexture(OglTexture *tex);

	virtual bool init();

	virtual void render();

	virtual	void deinit();

private:
	OglTexture *m_RenderTexture=NULL;
	Shader      *m_shader=NULL;
	FrameBufferObject *m_fbo;
	OglTexture *m_LogoTexture;
	glm::mat4   m_mvp;
};


#endif