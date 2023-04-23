#include "LogoRenderer.h"
#include "BstLogoArray.h"

LogoRenderer::LogoRenderer()
	: m_shader(NULL)
	, m_fbo(NULL)
	, m_RenderTexture(NULL)
	, m_LogoTexture(NULL)
{
	m_mvp = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 1.f));
}

LogoRenderer::~LogoRenderer()
{
	deinit();
}

void LogoRenderer::setInputTexture(OglTexture *tex)
{
	m_RenderTexture = tex;
	glBindTexture(m_RenderTexture->getTarget(), m_RenderTexture->getTextureId());
	glTexParameteri(m_RenderTexture->getTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_RenderTexture->getTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_RenderTexture->getTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_RenderTexture->getTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(m_RenderTexture->getTarget(), 0);
}

bool LogoRenderer::init()
{
	if(m_shader == NULL)
		m_shader = new LogoShader();
	m_shader->compile();

	if (m_fbo == NULL)
	{
		m_fbo = new FrameBufferObject();
	}
    initVao();
	m_LogoTexture = OglTextureFactory::createCommonTexture(logo_img_rgba_width, logo_img_rgba_height,
		OglTexture::_PIXEL_FORMAT_RGBA_8888, bst_logo_rgba);
	m_LogoTexture->updateTexture(bst_logo_rgba);
	return true;
}

void LogoRenderer::render()
{
	if (m_RenderTexture == NULL)
		return;

	m_fbo->attachTexture(m_RenderTexture);
	m_fbo->bind();

	glViewport(120, 20, logo_img_rgba_width*0.25, logo_img_rgba_height*0.25);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_mvp = glm::scale(glm::mat4(1.0f),
		glm::vec3(1.f,
			1.f,
			1.f));

	m_shader->use();
	m_shader->setMat4("MVP", m_mvp);

	glActiveTexture(GL_TEXTURE0);
	m_shader->setInt("tex", 0);

	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_LogoTexture->getTextureId());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);
	glBindVertexArray(0);
	m_fbo->unbind();

}

void LogoRenderer::deinit()
{
	if (m_vao != GL_INVALID_VALUE)
	{
		glDeleteVertexArrays(1, &m_vao);
		m_vao = GL_INVALID_VALUE;
	}

	if (m_vbo != GL_INVALID_VALUE)
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = GL_INVALID_VALUE;
	}

	if (m_ebo != GL_INVALID_VALUE)
	{
		glDeleteBuffers(1, &m_ebo);
		m_ebo = GL_INVALID_VALUE;
	}

	if (m_shader)
	{
		delete m_shader;
		m_shader = NULL;
	}

	if (m_LogoTexture)
	{
		delete m_LogoTexture;
		m_LogoTexture = NULL;
	}
	if (m_fbo)
	{
		delete m_fbo;
		m_fbo = NULL;
	}
}
