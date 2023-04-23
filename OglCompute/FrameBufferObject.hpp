#ifndef __FRAMEBUFFEROBJECT_HPP__
#define __FRAMEBUFFEROBJECT_HPP__

#include "OglTexture.hpp"

class FrameBufferObject
{
public:
    FrameBufferObject();

    ~FrameBufferObject();

    void attachTexture(OglTexture *texture);

    bool readTexture(unsigned char* buffer);

    void bind();

    void unbind();

private:
    GLuint      m_curFbo;
    GLint		m_defaultFbo;
    OglTexture  *m_texture;
};

#endif