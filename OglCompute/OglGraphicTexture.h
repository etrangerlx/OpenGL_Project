#ifndef __HARDWARE_TEXTURE_ANDROID8953_H__
#define __HARDWARE_TEXTURE_ANDROID8953_H__

//#include "HardwareTexture.h"
//#include <android/hardware_buffer.h>
#if defined(ANDROID)
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

//#include <android/hardware_buffer.h>

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>
//#include "GraphicBuffer.h"
#include "OglTexture.hpp"
#include "FrameBufferObject.hpp"
#include <map>
using std::map;

typedef struct 
{
    EGLImageKHR imageKHR;
	ogl_graphic_buffer* graphicbuffer=nullptr;

}GraphicKHRImage;

class GraphicTextureAndroid : public OglTexture 
{
public:
	GraphicTextureAndroid(int w, int h, void *buffer, OglGraphicCallBack* oglCallBack);
    GraphicTextureAndroid(int w, int h, void *buffer, OglGraphicCallBack* oglCallBack, GLuint texId);
	~GraphicTextureAndroid();
	virtual unsigned int getTextureId();
	virtual unsigned int getTextureFormat();
	virtual unsigned int getTarget();
	virtual bool bind();
	virtual void unbind();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getStride();
	virtual void sync();
	virtual bool needRead();
	virtual void unlock();
	virtual void lock(OglBufferPlanes* bufferPlanes);
	virtual void updateTexture(unsigned char *buffer);
	virtual void getPitch(int _pitch[4]);
	virtual void lockPlane(unsigned char *_plane[4]);
	// virtual int getFormat();

private:
	EGLImageKHR m_eglImageKHR=0;
	bool m_needDelete;
	FrameBufferObject *m_fbo=nullptr;
	uint32_t m_ionfd;
	void * m_native_handle=nullptr;
	map<void*,GraphicKHRImage> m_mapGraphicKHRImage;
	map<EGLClientBuffer,EGLImageKHR> m_mapImageKHR;
	GraphicKHRImage m_graphicKHR;
	//map<private_image_info*,EGLImageKHR> m_mapPriHandleKHR;
	OglGraphicCallBack_t* callBackFunc;
	EGLClientBuffer m_NativeBuf;
};
#endif
#endif