#ifndef __OGLBASEMEM_HPP__
#define __OGLBASEMEM_HPP__

#include "OpenGLConfig.h"
class OglBaseMem
{
public:
	OglBaseMem(GLuint index,GLuint64 size,void* buf):
		m_bindidx(index),
        m_bufferSize(size),
        m_ssboid(0),
        m_target(GL_SHADER_STORAGE_BUFFER),
		m_buf(buf){}

    virtual GLuint getSSBOId()
    {
        return m_ssboid;
    }
    
	virtual ~OglBaseMem()
	{
		
	}

    virtual void setSSBOId(GLuint id)
    {
        m_ssboid = id;
    }

    virtual void bind(void)
    {
    }

    virtual void unbind(void)
    {
    }

    virtual GLuint getTarget()
    {
        return m_target;
    }

    virtual int getBufferSize()
    {
        return m_bufferSize;
    }

    virtual GLuint getBindingIdx()
    {
        return m_bindidx;
    }

    virtual void setBindingIdx(GLuint idx)
    {
        m_bindidx = idx;
    }

    virtual void update(void* pData)
    {

    }

    virtual void* lock(int isWriteOnly) = 0;
    virtual void unlock() = 0;

protected:
	GLuint m_bindidx;
    GLuint m_ssboid;
    GLuint m_target;
	GLuint64 m_bufferSize;
    int needDelete = 0;
	void* m_buf;
};

class OglBaseMemFactory
{
public:
    static OglBaseMem* createCommonSSBO(GLuint index, GLuint64 size, void* buf=nullptr);
};
#endif
