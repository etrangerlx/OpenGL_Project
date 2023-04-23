#ifndef __EGLCONTENT_HPP__
#define __EGLCONTENT_HPP__

class EGLContent
{
public:
    virtual void setup() = 0;
    virtual void bindContext() = 0;
    virtual void unbindContext() = 0;
    virtual void swapBuffer() = 0;
    virtual void destory() = 0; 
    virtual void* getShareContext() = 0;
};

class EGLContentFactory
{
public:
#ifdef ANDROID
    static EGLContent* createEGLContentForAndroid();
#else
    static EGLContent* createEGLContentForWindows();
#endif
};

#endif