//#include "Define.hpp"
#if defined(ANDROID)
#include "OglGraphicTexture.h"
#include "OglTexture.hpp"
#include "FrameBufferObject.hpp"
//#include "../Util/TimeStatistic.h"
#define HAL_PIXEL_FORMAT_Y8 0x20203859
#if 1
#define CHECK  
#else
#define CHECK                                                                                                           \
    {                                                                                                                   \
        GLenum error = glGetError();                                                                                    \
        GLint error2 = eglGetError();                                                                                   \
        if (error != GL_NO_ERROR || error2 != EGL_SUCCESS){                                                             \
            printf("error !! line = %d gl code: %d,esgl code = %d file: %s,func: %s \n", __LINE__, error, error2, __FILE__, __FUNCTION__); \
            LOGI("error !! line = %d gl code: %d,esgl code = %d ,func: %s \n", __LINE__, error, error2, __FUNCTION__);\
        }\
    }
#endif
#define OGLKHR_CACHE_NUM 20
GraphicTextureAndroid::GraphicTextureAndroid(int w, int h, void *buffer, OglGraphicCallBack* oglCallBack, GLuint textureId)
{
    m_target = GL_TEXTURE_EXTERNAL_OES;
    EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);CHECK;
    m_eglImageKHR=0;
    m_id = textureId;

    m_width = w;
    m_height = h;
    m_target = GL_TEXTURE_EXTERNAL_OES;
    callBackFunc = oglCallBack;
    glGenTextures(1, &m_id);CHECK;
    LOGI("m_id = %d",m_id);
    void* nativeHandle =((private_image_info*)buffer)->native_handle;
	LOGI("w h s = %d %d %d",((private_image_info*)buffer)->width,((private_image_info*)buffer)->height,((private_image_info*)buffer)->stride);
    m_native_handle = nativeHandle;
   // memcpy(&m_PriHandle,nativeHandle,sizeof(private_image_info));
    LOGI("nativeHandle = %p,callBackFunc->userdata = %p",nativeHandle,callBackFunc->userdata);
    m_graphicKHR.graphicbuffer =  (ogl_graphic_buffer*)callBackFunc->createBuffer(buffer,callBackFunc->userdata);CHECK;
	if(m_graphicKHR.graphicbuffer == nullptr){
        printf("createNativebuffer failed");
	}
    EGLClientBuffer clientBuf = (EGLClientBuffer)callBackFunc->getClientBuf(m_graphicKHR.graphicbuffer, callBackFunc->userdata);CHECK;
    LOGI("getClientBuf succeed");
    PFNEGLCREATEIMAGEKHRPROC pCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");CHECK;
    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    m_graphicKHR.imageKHR = pCreateImageKHR(eglDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);CHECK;
    m_eglImageKHR =  m_graphicKHR.imageKHR;
    if(m_eglImageKHR == EGL_NO_IMAGE_KHR){
        printf("eglCreateImageKHR failed");
    }
    glBindTexture(m_target, m_id);CHECK;
    glEGLImageTargetTexture2DOES(m_target, m_eglImageKHR);CHECK;
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_target, 0);
    m_needDelete = false;CHECK;
    m_mapGraphicKHRImage[nativeHandle] = m_graphicKHR;

}

GraphicTextureAndroid::GraphicTextureAndroid(int w, int h, void *buffer, OglGraphicCallBack* oglCallBack)
{
    m_target = GL_TEXTURE_EXTERNAL_OES;
    EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);CHECK;
    m_eglImageKHR=0;
    m_id = GL_INVALID_VALUE;

    if (m_id != GL_INVALID_VALUE)
    {
        glDeleteTextures(1, &m_id);
        m_id = GL_INVALID_VALUE;
    }

    m_width = w;
    m_height = h;
    m_target = GL_TEXTURE_EXTERNAL_OES;
    callBackFunc = oglCallBack;
    glGenTextures(1, &m_id);CHECK;
    LOGI("m_id = %d",m_id);
    void* nativeHandle =((private_image_info*)buffer)->native_handle;
	LOGI("w h s = %d %d %d",((private_image_info*)buffer)->width,((private_image_info*)buffer)->height,((private_image_info*)buffer)->stride);
    m_native_handle = nativeHandle;
   // memcpy(&m_PriHandle,nativeHandle,sizeof(private_image_info));
    LOGI("nativeHandle = %p,callBackFunc->userdata = %p",nativeHandle,callBackFunc->userdata);
    m_graphicKHR.graphicbuffer =  (ogl_graphic_buffer*)callBackFunc->createBuffer(buffer,callBackFunc->userdata);CHECK;
	if(m_graphicKHR.graphicbuffer == nullptr){
        printf("createNativebuffer failed");
	}
    EGLClientBuffer clientBuf = (EGLClientBuffer)callBackFunc->getClientBuf(m_graphicKHR.graphicbuffer, callBackFunc->userdata);CHECK;
    LOGI("getClientBuf succeed");
    PFNEGLCREATEIMAGEKHRPROC pCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");CHECK;
    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    m_graphicKHR.imageKHR = pCreateImageKHR(eglDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);CHECK;
    m_eglImageKHR =  m_graphicKHR.imageKHR;
    if(m_eglImageKHR == EGL_NO_IMAGE_KHR){
        printf("eglCreateImageKHR failed");
    }
    glBindTexture(m_target, m_id);CHECK;
    glEGLImageTargetTexture2DOES(m_target, m_eglImageKHR);CHECK;
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_target, 0);
    m_needDelete = true;CHECK;
    m_mapGraphicKHRImage[nativeHandle] = m_graphicKHR;

}
GraphicTextureAndroid::~GraphicTextureAndroid()
{
    if (m_needDelete)
    {
        if(m_id!=GL_INVALID_VALUE)
        {
            glDeleteTextures(1, &m_id);CHECK
        }
        m_id = GL_INVALID_VALUE;
    }
    EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);CHECK
    map<void*,GraphicKHRImage>::iterator iterGraphicKHR;
    for (iterGraphicKHR=m_mapGraphicKHRImage.begin(); iterGraphicKHR!=m_mapGraphicKHRImage.end(); iterGraphicKHR++)
    {
        if(iterGraphicKHR->second.imageKHR!=0)
        {
            PFNEGLDESTROYIMAGEKHRPROC pDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");
            pDestroyImageKHR(eglDisplay, iterGraphicKHR->second.imageKHR);CHECK
        }
        LOGI("destory m_mapImageKHR[%p]\n",iterGraphicKHR->first);
        callBackFunc->destoryBuffer(iterGraphicKHR->second.graphicbuffer,callBackFunc->userdata);  
    }
    m_mapGraphicKHRImage.clear();
    m_eglImageKHR = 0;
}

void GraphicTextureAndroid::updateTexture(unsigned char *buffer)
{
    {
        LOGI("using graphic callback buffer  = %p ",((private_image_info*)buffer)->native_handle);
        void* native_handle =((private_image_info*)buffer)->native_handle;
        EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);CHECK
		
		/********** if need fd ***********/
		// if(priHandle->ion_fd < 0)
	    // {
		// 	PFNEGLCREATEIMAGEKHRPROC pCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
        //     EGLint eglImageAttributes[] = {EGL_WIDTH, m_width,EGL_HEIGHT,m_height, EGL_NONE};
        //     Timer::start();
        //     m_graphicKHR.graphicbuffer =  (ogl_graphic_buffer*)callBackFunc->createBuffer(priHandle,callBackFunc->userdata);
        //     EGLClientBuffer clientBuf = (EGLClientBuffer)callBackFunc->getClientBuf(m_graphicKHR.graphicbuffer, callBackFunc->userdata);CHECK;
        //     m_eglImageKHR = pCreateImageKHR(eglDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);CHECK;
        //     m_graphicKHR.imageKHR = m_eglImageKHR;
        //     Timer::stop("pCreateImageKHR");
        //     m_PriHandle = *priHandle;
        //     m_mapGraphicKHRImage[priHandle->ion_fd] = m_graphicKHR;
		// }
		// else
		{
            bool isFind=false;
            if(native_handle != m_native_handle)
            {
                map<void *,GraphicKHRImage>::iterator iterGraphicKHR;
                for(iterGraphicKHR = m_mapGraphicKHRImage.begin();iterGraphicKHR!=m_mapGraphicKHRImage.end();iterGraphicKHR++)
                {
                    if(iterGraphicKHR->first==native_handle)
                    {
                        isFind=true;
                        m_native_handle = native_handle;
                        m_graphicKHR = iterGraphicKHR->second;
                        m_eglImageKHR = m_graphicKHR.imageKHR;
    
                        break;
                    }
                }
                if (!isFind)
                {       
                    LOGI("find no fd index"); 
                    if(m_mapGraphicKHRImage.size() > OGLKHR_CACHE_NUM)
                    {
                        if(m_mapGraphicKHRImage.begin()->second.imageKHR!=0)
                        {
                            PFNEGLDESTROYIMAGEKHRPROC pDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");
                            pDestroyImageKHR(eglDisplay, m_mapGraphicKHRImage.begin()->second.imageKHR);CHECK
                        }
                        LOGI("destory m_mapImageKHR[%d]\n",m_mapGraphicKHRImage.begin()->first);
                        callBackFunc->destoryBuffer(m_mapGraphicKHRImage.begin()->second.graphicbuffer,callBackFunc->userdata);CHECK;
                        m_mapGraphicKHRImage.erase(m_mapGraphicKHRImage.begin());    
                    } 
                    PFNEGLCREATEIMAGEKHRPROC pCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
                    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
                    m_graphicKHR.graphicbuffer =  (ogl_graphic_buffer*)callBackFunc->createBuffer(buffer,callBackFunc->userdata);
                    EGLClientBuffer clientBuf = (EGLClientBuffer)callBackFunc->getClientBuf(m_graphicKHR.graphicbuffer, callBackFunc->userdata);CHECK;
                    m_eglImageKHR = pCreateImageKHR(eglDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID, clientBuf, eglImageAttributes);CHECK;
                    m_graphicKHR.imageKHR = m_eglImageKHR;
                    m_native_handle = native_handle;
                    m_mapGraphicKHRImage[native_handle] = m_graphicKHR;
                }
                else
                {
                    m_eglImageKHR = m_mapGraphicKHRImage[native_handle].imageKHR;
                    LOGI("using fd :%d",native_handle);
                }
            }
		}
    }
    glBindTexture(m_target, m_id);
    glEGLImageTargetTexture2DOES(m_target, m_eglImageKHR);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(m_target, 0);
    //m_mapImageKHR[m_NativeBuf] = m_eglImageKHR;
}

void GraphicTextureAndroid::lockPlane(unsigned char *_plane[4])
{
    int stat = callBackFunc->lock(m_graphicKHR.graphicbuffer,0,callBackFunc->userdata);
    m_stride = m_graphicKHR.graphicbuffer->pitch[0];
    plane[0]=(unsigned char *)m_graphicKHR.graphicbuffer->plane[0];
    plane[1]=(unsigned char *)m_graphicKHR.graphicbuffer->plane[1];
    _plane[0] = plane[0];
    _plane[1] = plane[1];
    pitch[0] = m_graphicKHR.graphicbuffer->pitch[0];
    pitch[1] = m_graphicKHR.graphicbuffer->pitch[1];
    m_stride = m_graphicKHR.graphicbuffer->pitch[0];
}

void GraphicTextureAndroid::lock(OglBufferPlanes* bufferPlanes)
{
    int stat = callBackFunc->lock(m_graphicKHR.graphicbuffer, 0, callBackFunc->userdata);
    bufferPlanes->planes[0].data = (unsigned char*)m_graphicKHR.graphicbuffer->plane[0];
    bufferPlanes->planes[1].data = (unsigned char*)m_graphicKHR.graphicbuffer->plane[1];
    plane[0] = (unsigned char *)bufferPlanes->planes[0].data;
    plane[1] = (unsigned char*)bufferPlanes->planes[1].data;
    plane[2] = (unsigned char*)bufferPlanes->planes[2].data;
    plane[3] = (unsigned char*)bufferPlanes->planes[3].data;
    pitch[0] = m_graphicKHR.graphicbuffer->pitch[0];
    pitch[1] = m_graphicKHR.graphicbuffer->pitch[1];
    m_stride = m_graphicKHR.graphicbuffer->pitch[0];
    bufferPlanes->planes[0].pixelStride = m_stride;
    bufferPlanes->planes[0].rowStride = m_stride;

    return;
}

void GraphicTextureAndroid::unlock()
{
    callBackFunc->unlock(m_graphicKHR.graphicbuffer, callBackFunc->userdata);
    //glBindTexture(m_target, 0);
}

void GraphicTextureAndroid::getPitch(int _pitch[4])
{
    _pitch[0] = pitch[0];
    _pitch[1] = pitch[1];
}
unsigned int GraphicTextureAndroid::getTextureId()
{
    return m_id;
}

unsigned int GraphicTextureAndroid::getTextureFormat()
{
    return 0;
}

unsigned int GraphicTextureAndroid::getTarget()
{
    return m_target;
}

bool GraphicTextureAndroid::bind()
{
    if (m_id == GL_INVALID_VALUE)
        return false;
    glBindTexture(m_target, m_id);
    glEGLImageTargetTexture2DOES(m_target, m_eglImageKHR);
    return true;
}

void GraphicTextureAndroid::unbind()
{
    glBindTexture(m_target, 0);CHECK;
   // glEGLImageTargetTexture2DOES(m_target, 0);CHECK;
}

int GraphicTextureAndroid::getWidth()
{
    return m_width;
}

int GraphicTextureAndroid::getHeight()
{
    return m_height;
}

int GraphicTextureAndroid::getStride()
{
    return m_stride;
}



void GraphicTextureAndroid::sync()
{
}

bool GraphicTextureAndroid::needRead()
{
    return false;
}


OglTexture *OglTextureFactory::createGraphicTextureAndroid(int w, int h, void *buffer, void* oglCallBack)
{
    return new GraphicTextureAndroid(w, h, buffer,(OglGraphicCallBack*)oglCallBack);
}
OglTexture *OglTextureFactory::createGraphicTextureAndroid(int w, int h, void *buffer, void* oglCallBack,GLuint texId)
{
    return new GraphicTextureAndroid(w, h, buffer,(OglGraphicCallBack*)oglCallBack,texId);
}
#endif