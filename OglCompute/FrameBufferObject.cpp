#include "FrameBufferObject.hpp"
#include "Define.hpp"
FrameBufferObject::FrameBufferObject()
        : m_curFbo(0)
        , m_defaultFbo(0)
        , m_texture(nullptr)
{
    glGenFramebuffers(1, &m_curFbo);
}

FrameBufferObject::~FrameBufferObject()
{
    if (m_curFbo != 0)
    {
        glDeleteFramebuffers(1, &m_curFbo);
		m_curFbo = 0;
    }
}

void FrameBufferObject::attachTexture(OglTexture *texture)
{
    if (texture == NULL)
        return;
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_defaultFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_curFbo);
	//LOGI("after glBindFramebuffer glError (0x%x)\n", glGetError());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                    texture->getTarget(), texture->getTextureId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFbo);
    m_texture = texture;
}

bool FrameBufferObject::readTexture(unsigned char* buffer)
{
    if (m_texture->needRead())
    {
        GLint oldFbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);CHECK;
        glBindFramebuffer(GL_FRAMEBUFFER, m_curFbo);CHECK;
        glReadBuffer(GL_COLOR_ATTACHMENT0);CHECK;
        glReadPixels(0, 0, m_texture->getWidth(), m_texture->getHeight(), m_texture->getColorFormat(), m_texture->getType(), buffer);CHECK;
        printf("getType() == 0x%x getColorFormat() = 0x%x\n",m_texture->getType(),m_texture->getColorFormat());
        glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);CHECK;
    }
    
    return true;
}

void FrameBufferObject::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_curFbo);
}

void FrameBufferObject::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFbo);
}
