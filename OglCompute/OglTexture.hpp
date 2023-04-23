#ifndef __OGLTEXTURE_HPP__
#define __OGLTEXTURE_HPP__

#include "OpenGLConfig.h"
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Define.hpp"

typedef struct
{
    int width;
    int height;
    int fmt;
    int pitch[4];
    void *plane[4];
    void *graphic_buffer;
    void *user_data;
} ogl_graphic_buffer;

typedef enum
{
       NATIVE_HANDLE_TYPE = 0, //qcom
       AHARDWARE_TYPE = 1, //mtk
 GRAPHIC_BUFFER_TYPE = 2, //
} privateHdlType;

typedef struct {
 int width;
 int height;
 int stride;
 void *native_handle;
} private_image_info;
/*
typedef struct 
{
    int width;
    int height;
    int stride;
    void *native_handle;
} private_image_info;
*/

typedef struct OglGraphicCallBack_t
{
    typedef ogl_graphic_buffer *(*createGraphicBufferFunc)(void *priv_handle, void *userdata);
    typedef void (*destoryBufferFunc)(ogl_graphic_buffer *gb, void *userdata);
    typedef int (*lockFunc)(ogl_graphic_buffer *gb, int for_write, void *userdata);
    typedef int (*unlockFunc)(ogl_graphic_buffer *gb, void *userdata);
    typedef void *(*getClientBufFunc)(ogl_graphic_buffer *gb, void *userdata);
    createGraphicBufferFunc createBuffer; // function to create graphic buffer
    destoryBufferFunc destoryBuffer;	  // function to destory graphic buffer
    lockFunc lock;						  // function to get cpu data ptr(nv21)
    unlockFunc unlock;
    getClientBufFunc getClientBuf;
    void *userdata; //william add for reserved
} OglGraphicCallBack;

class OglTexture
{
public:
    typedef enum
    {
        /* defined for texture enum start id*/
        _PIXEL_FORMAT_START_ID = 0,
        /* defined for common texture and hardware texture*/
        _PIXEL_FORMAT_RGBA_8888,
        _PIXEL_FORMAT_RGB_888,
        _PIXEL_FORMAT_R_8,
        _PIXEL_FORMAT_RG_88,
        _PIXEL_FORMAT_LUMINANCE,
        _PIXEL_FORMAT_LUMINANCE_ALPHA,
        _PIXEL_FORMAT_ALPHA,
        /* defined for hardware external texture only*/
        _PIXEL_FORMAT_NV21,
        _PIXEL_FORMAT_NV12,
        _PIXEL_FORMAT_RGBA16I,
        _PIXEL_FORMAT_RGB16I,
        _PIXEL_FORMAT_RGB_INTEGER,  
        _PIXEL_FORMAT_RGBA_INTEGER,
        _PIXEL_FORMAT_RGBA32F,
        /* defined for texture enum end id*/
        _PIXEL_FORMAT_END_ID,        
    }PIX_FORMAT;

     enum TexTure_BYTE
    {
        E_Type_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        E_Type_BYTE = GL_BYTE,
        E_Type_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        E_Type_SHORT = GL_SHORT,
        E_Type_INT = GL_INT,
        E_Type_UNSIGNED_INT = GL_UNSIGNED_INT,
        E_Type_FLOAT = GL_FLOAT
    };

    std::map<unsigned int, unsigned int> BYTE_SIZE{
        {E_Type_UNSIGNED_BYTE,     sizeof(char) },
        {E_Type_BYTE,              sizeof(char) },
        {E_Type_UNSIGNED_SHORT,    sizeof(short)},
        {E_Type_SHORT,             sizeof(short)},
        {E_Type_INT,               sizeof(int) },
        {E_Type_UNSIGNED_INT,      sizeof(int) },
        {E_Type_FLOAT,             sizeof(float)}
    };
    
    typedef struct OglBufferPlane {
        void*       data;        ///< Points to first byte in plane
        uint32_t    pixelStride; ///< Distance in bytes from the color channel of one pixel to the next
        uint32_t    rowStride;   ///< Distance in bytes from the first value of one row of the image to
                                ///  the first value of the next row.
    } OglBufferPlane;

    /**
     * Holds all image planes that contain the pixel data.
     */
    typedef struct OglBufferPlanes {
        uint32_t               planeCount; ///< Number of distinct planes
        OglBufferPlane  planes[4];     ///< Array of image planes
    } OglBufferPlanes;
    OglTexture(){}

    OglTexture(int w, int h, PIX_FORMAT pixelFmt,PIX_FORMAT inPixelFmt, GLuint id = 0)
            : m_target(GL_TEXTURE_2D)
            , m_id(id)
            , m_width(w)
            , m_height(h)
            , m_pixelFmt(pixelFmt)
            , m_inPxelFmt(pixelFmt)
            , m_bufferSize(w * h * 4)
            , m_needRead(true)
    {

    }
     OglTexture(int w, int h, PIX_FORMAT pixelFmt, GLuint id = 0)
            : m_target(GL_TEXTURE_2D)
            , m_id(id)
            , m_width(w)
            , m_height(h)
            , m_pixelFmt(pixelFmt)
            , m_inPxelFmt(pixelFmt)
            , m_bufferSize(w * h * 4)
            , m_needRead(true)
    {

    }

    virtual ~OglTexture()
    {}
    
    virtual GLuint getTextureId()
    {
        return m_id;
    }

    virtual void setTextureId(GLuint id)
    {
        m_id = id;
    }

    virtual GLuint getTextureFormat()
    {
        return m_textureFmt;
    }

    virtual GLuint getColorFormat()
    {
        return m_inPixDataFmt;
    }

    virtual GLuint getTarget()
    {
        return m_target;
    }

    virtual GLuint getType()
    {
        return (GLuint)m_pixelType;
    }

    virtual void lockPlane(unsigned char* _plane[4])
    {
        OglBufferPlanes bufferPlanes;
       lock(&bufferPlanes);
       plane[3] = plane[2] = plane[1] = plane[0] = (unsigned char *)bufferPlanes.planes[0].data;
        _plane[0] = plane[0];
        _plane[1] = plane[1];
        _plane[2] = plane[2];
        _plane[3] = plane[3];
    }

    virtual void getPitch(int _pitch[4])
    {
        pitch[3] = pitch[2] = pitch[1] = pitch[0] = m_stride;
        _pitch[0] = pitch[0];
        _pitch[1] = pitch[1];
        _pitch[2] = pitch[2];
        _pitch[3] = pitch[3];
    }

    virtual int getStride()
    {
        return m_stride;
    }

    virtual bool bind() = 0;

    virtual void unbind() = 0;

    virtual int getWidth()
    {
        return m_width;
    }

    virtual int getHeight()
    {
        return m_height;
    }


    virtual int getBufferSize()
    {
        return m_bufferSize;
    }

    virtual void lock(OglBufferPlanes* bufferPlanes) = 0;

    virtual void unlock() = 0;

    virtual void updateTexture(unsigned char* buffer) = 0;

    virtual bool needRead()
    {
        return m_needRead;
    }

protected:
    OglBufferPlanes m_bufferPlanes;
    GLuint m_textureFmt;
    GLuint m_inPixDataFmt;
    PIX_FORMAT m_pixelFmt;
    PIX_FORMAT m_inPxelFmt;
    GLuint m_pixelType;
    GLuint m_target;
    GLuint m_id;
    int m_width;
    int m_height;
    int m_stride;
    int m_bufferSize;
    bool m_needRead;
    unsigned char *plane[4]={0,0,0,0};
	int pitch[4]={0,0,0,0};
};

class OglTextureFactory
{
public:
    static OglTexture* createCommonTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt, unsigned char* buf = nullptr);
    static OglTexture* createCommonTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt, GLuint id);
    //static OglTexture* createCommonTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt, unsigned char* buf = nullptr,unsigned int byteType = 8);
#if (defined ANDROID) && (__ANDROID_API__ >= 29)
	static OglTexture* createHardwareTexture(int w, int h, OglTexture::PIX_FORMAT pixelFmt);
#endif
#if defined(ANDROID)
    static OglTexture* createGraphicTextureAndroid(int w, int h, void* buffer, void* oglCallBack);
	static OglTexture* createGraphicTextureAndroid(int w, int h, void* buffer, void* oglCallBack,GLuint textureId);
#endif
};

#endif