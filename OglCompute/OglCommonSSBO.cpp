#include <stdio.h>
#include <iostream>
#include "OglBaseMem.hpp"
#include "Define.hpp"
class OglCommonSSBO :public OglBaseMem
{
public:
	OglCommonSSBO(GLuint index, GLuint64 size, void* buf)
		:OglBaseMem(index, size, buf)
	{
		glGenBuffers(1, &m_ssboid);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssboid);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, buf, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ssboid);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		needDelete = 1;
		m_target = GL_SHADER_STORAGE_BUFFER;


	}
	~OglCommonSSBO()
	{
		if (needDelete)
		{
			glDeleteBuffers(1, &m_ssboid);
		}
	}

	void bind()
	{
		glBindBuffer(m_target, m_ssboid); CHECK;
		glBindBufferBase(m_target, m_bindidx, m_ssboid); CHECK;
	}
	void unbind()
	{
		glBindBuffer(m_target, 0);CHECK;
		glBindBufferBase(m_target, m_bindidx, 0); CHECK;
	}

	void* lock(int isWriteOnly)
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); CHECK;
		glBindBuffer(m_target, m_ssboid);
		//glBindBufferBase(m_target, m_bindidx, m_ssboid); CHECK;
		void* tmp = nullptr;
		if (!isWriteOnly)
		{
			tmp = (void*)glMapBufferRange(m_target, 0, m_bufferSize, GL_MAP_READ_BIT); CHECK;
		}
		else	
		{ 
			tmp = (void*)glMapBufferRange(m_target, 0, m_bufferSize, GL_MAP_WRITE_BIT); CHECK;
		}
		return tmp;
	}
	void unlock()
	{
		glUnmapBuffer(m_target); CHECK;
	}
	void update(void* pData)
	{
		void* tmp = lock(1);
		memcpy(tmp, pData, m_bufferSize);
		unlock();
	}
};

OglBaseMem* OglBaseMemFactory::createCommonSSBO(GLuint index, GLuint64 size, void* buf)
{
		return new OglCommonSSBO(index, size, buf);
}