#include <stdio.h>
#include <iostream>

#include "OglTexture.hpp"
#include "FrameBufferObject.hpp"


#ifndef LOGI

#ifdef ANDROID
#include <android/log.h>
#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "OpenglesCompute", "[%s : %d] " fmt, __FUNCTION__, __LINE__,##__VA_ARGS__)
#else
#define LOGI(...)    do{printf("[%s : %d] : ", __FUNCTION__, __LINE__);printf(__VA_ARGS__); printf("\n");}while(0)
#endif

#endif

//#define DEBUG__PRINT
#if defined(DEBUG__PRINT)
#define CHECK                                                                                 \
    {        \
        glFinish();                                                                                \
        GLenum error = (glGetError());                                                        \
        if (error != GL_NO_ERROR){                                                             \
            printf("error !! line = %d code: %d func: %s \n", __LINE__, error, __FUNCTION__); \
            LOGI("error !! line = %d code: %d func: %s ", __LINE__, error, __FUNCTION__);\
        }\
    }
#endif
class OglCommonTexture : public OglTexture
{
public:
    OglCommonTexture(int w, int h, PIX_FORMAT fmt, unsigned char* buf)
        :OglTexture(w, h, fmt, fmt)
        , m_buffer(nullptr)
    {
		m_stride = w;
        m_fbo = new FrameBufferObject();
       // m_pixelType = m_ByteType;
        //printf("m_ByteType = %x \n",m_ByteType);
        m_bufferPlanes.planeCount = 1;

        if (fmt == _PIXEL_FORMAT_RGB_888)
        {
            m_inPixDataFmt = m_textureFmt = GL_RGB;
            m_bufferSize = m_width * m_height * 3;
            m_bufferPlanes.planes[0].pixelStride = 3;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_UNSIGNED_BYTE;
        }
        else if(fmt == _PIXEL_FORMAT_R_8)
        {
            m_inPixDataFmt = m_textureFmt = GL_RED;
            m_bufferSize = m_width * m_height;
            m_bufferPlanes.planes[0].pixelStride = 1;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_UNSIGNED_BYTE;
        }
        else if(fmt == _PIXEL_FORMAT_LUMINANCE)
        {
            m_inPixDataFmt = m_textureFmt = GL_LUMINANCE;
            m_bufferSize = m_width * m_height;
            m_bufferPlanes.planes[0].pixelStride = 1;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_UNSIGNED_BYTE;
        }
        else if(fmt == _PIXEL_FORMAT_ALPHA)
        {
            m_inPixDataFmt = m_textureFmt = GL_ALPHA;
            m_bufferSize = m_width * m_height;
            m_bufferPlanes.planes[0].pixelStride = 1;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_UNSIGNED_BYTE;
        }                
        else if(fmt == _PIXEL_FORMAT_RG_88)
        {
            m_inPixDataFmt = m_textureFmt = GL_RG;
            m_bufferSize = m_width * m_height * 2;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_UNSIGNED_BYTE;
        }             
        else if(fmt == _PIXEL_FORMAT_LUMINANCE_ALPHA)
        {
            m_inPixDataFmt = m_textureFmt = GL_LUMINANCE_ALPHA;
            m_bufferSize = m_width * m_height * 2;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_UNSIGNED_BYTE;

        }   
        else if(fmt == _PIXEL_FORMAT_RGB16I)
        {
            m_textureFmt = GL_RGB16I;
            m_bufferSize = m_width * m_height * 6;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_SHORT;
            m_inPixDataFmt = GL_RGB_INTEGER;
        }   
        else if(fmt == _PIXEL_FORMAT_RGBA16I)
        {
            m_textureFmt = GL_RGBA16I;
            m_bufferSize = m_width * m_height * 8;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_SHORT;
            m_inPixDataFmt = GL_RGBA_INTEGER;
        }  
        else if (_PIXEL_FORMAT_RGBA32F == fmt)
        {
            m_textureFmt = GL_RGBA32F;
            m_bufferSize = m_width * m_height * 16;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_FLOAT;
            m_inPixDataFmt = GL_RGBA;
        }
        else 
        {
            m_textureFmt = GL_RGBA;
            m_bufferSize = m_width * m_height * 4;
            m_bufferPlanes.planes[0].pixelStride = 4;
            m_bufferPlanes.planes[0].rowStride = m_width;
			m_pixelType = GL_UNSIGNED_BYTE;
            m_inPixDataFmt = GL_RGBA;

        }
        m_ByteType = (TexTure_BYTE)m_pixelType;
        m_needDelete = true;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &m_id);
        glBindTexture(m_target, m_id);
       
         if(fmt == _PIXEL_FORMAT_RGBA_8888)
         {
            glTexStorage2D(m_target, 1, GL_RGBA8, m_width, m_height); CHECK;//// RGBA8888 computer shader
			if (buf != nullptr)
        	 {
             	updateTexture(buf);
         	 }
         }
         else 
         {
            glTexImage2D(m_target, 0, m_textureFmt, m_width, m_height, 0, m_inPixDataFmt, m_pixelType, buf); CHECK;// computer shader for other format
         }
        //glTexImage2D(m_target, 0, m_textureFmt, m_width, m_height, 0, m_textureFmt, GL_UNSIGNED_BYTE, buf);CHECK;
         
		 if(fmt == _PIXEL_FORMAT_RGB16I || fmt == _PIXEL_FORMAT_RGBA16I || fmt == _PIXEL_FORMAT_RGBA32F)
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);CHECK;
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);CHECK;
        }
		else
		{
        	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);CHECK;
        	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);CHECK;
		}
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//GL_CLAMP_TO_BORDER
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(m_target, 0);
        printf("m_id = %d \n",m_id);        
    }

    OglCommonTexture(int w, int h, PIX_FORMAT fmt, GLuint id)
        :OglTexture(w, h, fmt,fmt,id)
        , m_buffer(nullptr)
    {		
	    m_stride = w;
        //printf("m_ByteType = %x \n",m_ByteType);
		m_fbo = new FrameBufferObject();CHECK;
		glBindTexture(m_target, m_id);CHECK;
		if(fmt == _PIXEL_FORMAT_RGB16I || fmt == _PIXEL_FORMAT_RGBA16I)
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);CHECK;
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);CHECK;
        }
        else
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);CHECK;
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);CHECK;
        }
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);CHECK;	//GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);CHECK;
		glBindTexture(m_target, 0);CHECK;
        if (fmt == _PIXEL_FORMAT_RGBA_8888)
        {
            m_textureFmt = GL_RGBA;
            m_bufferSize = m_width * m_height * 4;
            m_bufferPlanes.planes[0].pixelStride = 4;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_RGBA;
            m_pixelType = GL_UNSIGNED_BYTE;
        }
        else if (fmt == _PIXEL_FORMAT_RGB_888)
        {
            m_textureFmt = GL_RGB;
            m_bufferSize = m_width * m_height * 3;
            m_bufferPlanes.planes[0].pixelStride = 3;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_RGB;
            m_pixelType = GL_UNSIGNED_BYTE;

        }
        else if(fmt == _PIXEL_FORMAT_R_8)
        {
            m_textureFmt = GL_RED;
            m_bufferSize = m_width * m_height;
            m_bufferPlanes.planes[0].pixelStride = 1;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_RED;
            m_pixelType = GL_UNSIGNED_BYTE;

        }
        else if(fmt == _PIXEL_FORMAT_LUMINANCE)
        {
            m_textureFmt = GL_LUMINANCE;
            m_bufferSize = m_width * m_height;
            m_bufferPlanes.planes[0].pixelStride = 1;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_LUMINANCE;
            m_pixelType = GL_UNSIGNED_BYTE;

        }
        else if(fmt == _PIXEL_FORMAT_ALPHA)
        {
            m_textureFmt = GL_ALPHA;
            m_bufferSize = m_width * m_height;
            m_bufferPlanes.planes[0].pixelStride = 1;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_ALPHA;
            m_pixelType = GL_UNSIGNED_BYTE;

        }                
        else if(fmt == _PIXEL_FORMAT_RG_88)
        {
            m_textureFmt = GL_RG;
            m_bufferSize = m_width * m_height * 2;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_RG;
            m_pixelType = GL_UNSIGNED_BYTE;

        }             
        else if(fmt == _PIXEL_FORMAT_LUMINANCE_ALPHA)
        {
            m_textureFmt = GL_LUMINANCE_ALPHA;
            m_bufferSize = m_width * m_height * 2;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_LUMINANCE_ALPHA;
            m_pixelType = GL_UNSIGNED_BYTE;

        }     
        else if(fmt == _PIXEL_FORMAT_RGB16I)
        {
            m_textureFmt = GL_RGB16I;
            m_bufferSize = m_width * m_height * 6;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_SHORT;
            m_inPixDataFmt = GL_RGB;
        }    
        else if(fmt == _PIXEL_FORMAT_RGBA16I)
        {
            m_textureFmt = GL_RGBA16I;
            m_bufferSize = m_width * m_height * 8;
            m_bufferPlanes.planes[0].pixelStride = 2;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_pixelType = GL_SHORT;
            m_inPixDataFmt = GL_RGBA;
        }                 
        else 
        {
            m_textureFmt = GL_RGBA;
            m_bufferSize = m_width * m_height * 4;
            m_bufferPlanes.planes[0].pixelStride = 4;
            m_bufferPlanes.planes[0].rowStride = m_width;
            m_inPixDataFmt = GL_RGBA;
        }
        m_ByteType = (TexTure_BYTE)m_pixelType;
        m_needDelete = false;
    }

    ~OglCommonTexture()
    {
        if (m_buffer != nullptr)
        {
            free(m_buffer);
            m_buffer = nullptr; 
        }

        if (m_needDelete)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }

		if (m_fbo)
		{
			delete m_fbo;
			m_fbo = NULL;
		}
    }
    virtual bool bind()
    {
        if (m_id == GL_INVALID_VALUE)
            return false;
        glBindTexture(m_target, m_id);
        return true;
    }

    virtual void unbind()
    {
        glBindTexture(m_target, 0);
    }
    
    virtual void lock(OglBufferPlanes* bufferPlanes)
    {
        if (m_buffer == nullptr)
            m_buffer = (unsigned char*)malloc(m_bufferSize);
        m_fbo->attachTexture(this);
		m_fbo->readTexture(m_buffer);
        m_bufferPlanes.planes[0].data = (void*)m_buffer;
        memcpy(bufferPlanes, &m_bufferPlanes, sizeof(m_bufferPlanes));
    }

    virtual void unlock()
    {
        //updateTexture(m_buffer);
    }

    virtual void updateTexture(unsigned char* buffer)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glBindTexture(m_target, m_id);
        glTexSubImage2D(m_target, 0, 0, 0, m_width, m_height, m_inPixDataFmt, m_pixelType, buffer);CHECK;
    }

private:
    bool m_needDelete;
    unsigned char *m_buffer;
    TexTure_BYTE m_ByteType = E_Type_UNSIGNED_BYTE;
    FrameBufferObject *m_fbo;
};


OglTexture* OglTextureFactory::createCommonTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt, unsigned char* buf)
{
    if((int)pixelFmt <= OglTexture::_PIXEL_FORMAT_START_ID
    || (int)pixelFmt >= OglTexture::_PIXEL_FORMAT_END_ID )
    {
        printf("format %d not support !!!\n",pixelFmt);
        LOGI("format %d not support !!!",pixelFmt);
        return NULL;
    }

    if(pixelFmt == OglTexture::_PIXEL_FORMAT_NV21
    || pixelFmt == OglTexture::_PIXEL_FORMAT_NV12)
    {
        printf("format %d not support !!!\n",pixelFmt);
        LOGI("format %d not support !!!",pixelFmt);
        return NULL;
    }
    if(buf == nullptr && ((int)pixelFmt > OglTexture::_PIXEL_FORMAT_START_ID && (int)pixelFmt < OglTexture::_PIXEL_FORMAT_NV12))
    {
       // inFmt = pixelFmt;
        //printf("pixelFmt = %x, buf = %x,inFmt = %x,byteType = %x\n",pixelFmt, buf ,inFmt,byteType);
    }
    return new OglCommonTexture(w, h, pixelFmt, buf);
}

OglTexture* OglTextureFactory::createCommonTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt, GLuint id)
{
    if((int)pixelFmt <= OglTexture::_PIXEL_FORMAT_START_ID
    || (int)pixelFmt >= OglTexture::_PIXEL_FORMAT_END_ID )
    {
        return NULL;
    }

    if(pixelFmt == OglTexture::_PIXEL_FORMAT_NV21
    || pixelFmt == OglTexture::_PIXEL_FORMAT_NV12)
    {
        return NULL;
    }
        
    return new OglCommonTexture(w, h, pixelFmt, id);
}
