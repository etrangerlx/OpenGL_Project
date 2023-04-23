#if (defined ANDROID) && (__ANDROID_API__ >= 29)

#include <stdio.h>
#include <iostream>
#include <android/hardware_buffer.h>
#include "OglTexture.hpp"
#include "FrameBufferObject.hpp"
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>


class OglHardWareTexture : public OglTexture
{
public:
    OglHardWareTexture(int w, int h, PIX_FORMAT pixelFmt)
        :OglTexture(w, h, pixelFmt)
    {
        m_needRead = false;
        EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        m_eglImageKHR = 0;      
        m_hardWareBuffer = NULL;
        m_id = GL_INVALID_VALUE;
        m_width = w;
        m_height = h;
        m_target = GL_TEXTURE_EXTERNAL_OES;
        //m_target = GL_TEXTURE_2D;

        m_desc.width = w;
        m_desc.height = h;
        m_desc.layers = 1;
        m_desc.rfu0 = 0;
        m_desc.rfu1 = 0;
        if (pixelFmt == _PIXEL_FORMAT_RGBA_8888)
        {
            m_desc.format = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM;
            m_textureFmt = GL_RGBA;
            m_bufferPlanes.planeCount = 1;
            m_bufferPlanes.planes[0].pixelStride = 4;
            m_desc.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN
            | AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_FRAMEBUFFER;
        }
        else if (pixelFmt == _PIXEL_FORMAT_RGB_888)
        {
            m_desc.format = AHARDWAREBUFFER_FORMAT_R8G8B8_UNORM;
            m_textureFmt = GL_RGB;
            m_bufferPlanes.planeCount = 1;
            m_bufferPlanes.planes[0].pixelStride = 3;
            m_desc.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN
            | AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_FRAMEBUFFER;
        }
        else if (pixelFmt == _PIXEL_FORMAT_NV21
        || pixelFmt == _PIXEL_FORMAT_NV12)
        {
            m_desc.format = AHARDWAREBUFFER_FORMAT_Y8Cb8Cr8_420;
            m_bufferPlanes.planeCount = 3;
            m_desc.usage = AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_FRAMEBUFFER | (0x00020000U);
        }
        // m_desc.usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN
        // | AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE | AHARDWAREBUFFER_USAGE_GPU_FRAMEBUFFER;

        AHardwareBuffer_allocate(&m_desc, &m_hardWareBuffer);

        PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC pGetNativeClientBufferANDROID = (PFNEGLGETNATIVECLIENTBUFFERANDROIDPROC)eglGetProcAddress("eglGetNativeClientBufferANDROID");
        EGLClientBuffer clientBuf = (EGLClientBuffer)pGetNativeClientBufferANDROID(m_hardWareBuffer);
        
        PFNEGLCREATEIMAGEKHRPROC pCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
        EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
        m_eglImageKHR = pCreateImageKHR(eglDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);

        glGenTextures(1, &m_id);
        glBindTexture(m_target, m_id);
        glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glEGLImageTargetTexture2DOES(m_target, m_eglImageKHR);
      
        glBindTexture(m_target, 0);

        if(pixelFmt != _PIXEL_FORMAT_NV21
        && pixelFmt != _PIXEL_FORMAT_NV12)
        {
            AHardwareBuffer_Desc desc;
            AHardwareBuffer_describe(m_hardWareBuffer, &desc);
            m_bufferPlanes.planes[0].rowStride = desc.stride;
        }

    }

    ~OglHardWareTexture()
    {
        if (m_eglImageKHR != 0)
        {
            EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            PFNEGLDESTROYIMAGEKHRPROC pDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");
            pDestroyImageKHR(eglDisplay, m_eglImageKHR);
        }

        if (m_hardWareBuffer != NULL)
        {
            AHardwareBuffer_release(m_hardWareBuffer);
            m_hardWareBuffer = NULL;
        }

        if (m_id != GL_INVALID_VALUE)
        {
            glDeleteTextures(1, &m_id);
            m_id = GL_INVALID_VALUE;
        }
    }

    virtual bool bind()
    {
        if (m_id == GL_INVALID_VALUE)
            return false;
        glBindTexture(m_target, m_id);
        glEGLImageTargetTexture2DOES(m_target, m_eglImageKHR);
        return true;
    }

    virtual void unbind()
    {
        glBindTexture(m_target, 0);
        //glEGLImageTargetTexture2DOES(m_target, (GLeglImageOES)0);
    }

        virtual void lock(OglBufferPlanes* bufferPlanes)
    {
        if(m_desc.format == AHARDWAREBUFFER_FORMAT_Y8Cb8Cr8_420)
        {
            AHardwareBuffer_Planes outPlanes;
            AHardwareBuffer_lockPlanes(m_hardWareBuffer, AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
            -1, nullptr, &outPlanes);
            
            m_bufferPlanes.planeCount = outPlanes.planeCount;
            m_bufferPlanes.planes[0].data = outPlanes.planes[0].data;
            m_bufferPlanes.planes[0].pixelStride = outPlanes.planes[0].pixelStride;
            m_bufferPlanes.planes[0].rowStride = outPlanes.planes[0].rowStride;

            m_bufferPlanes.planes[1].data = outPlanes.planes[1].data;
            m_bufferPlanes.planes[1].pixelStride = outPlanes.planes[1].pixelStride;
            m_bufferPlanes.planes[1].rowStride = outPlanes.planes[1].rowStride;  

            m_bufferPlanes.planes[2].data = outPlanes.planes[2].data;
            m_bufferPlanes.planes[2].pixelStride = outPlanes.planes[2].pixelStride;
            m_bufferPlanes.planes[2].rowStride = outPlanes.planes[2].rowStride;    

            m_bufferPlanes.planes[3].data = outPlanes.planes[3].data;
            m_bufferPlanes.planes[3].pixelStride = outPlanes.planes[3].pixelStride;
            m_bufferPlanes.planes[3].rowStride = outPlanes.planes[3].rowStride;
            memcpy(bufferPlanes, &m_bufferPlanes, sizeof(m_bufferPlanes));               
        }
        else
        {
            void *ptr = NULL;
            AHardwareBuffer_lock(m_hardWareBuffer, AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN | AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
            -1, nullptr, (void**)&ptr);
            m_bufferPlanes.planes[0].data = ptr;
            memcpy(bufferPlanes, &m_bufferPlanes, sizeof(m_bufferPlanes));       
        }
    }

    virtual void unlock()
    {
        AHardwareBuffer_unlock(m_hardWareBuffer, nullptr);
    }

    virtual void updateTexture(unsigned char* buffer)
    {

    }

private:
    AHardwareBuffer_Desc m_desc;
    EGLImageKHR m_eglImageKHR;
    AHardwareBuffer *m_hardWareBuffer;
};

// Note: work for rgba/nv21/nv12 format input
OglTexture* OglTextureFactory::createHardwareTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt)
{
    if((int)pixelFmt <= OglTexture::_PIXEL_FORMAT_START_ID
    || (int)pixelFmt >= OglTexture::_PIXEL_FORMAT_END_ID )
    {
        return NULL;
    }
   
    return new OglHardWareTexture(w, h, pixelFmt);
}
#endif
