#include "EGLContent.hpp"
#include "OpenGLConfig.h"
#ifdef ANDROID
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <unistd.h>
#include <string.h>
#define ERROR_ON_MSG(cond, ...)  \
    do                                      \
    {                                       \
        if(cond)                            \
        {                                   \
            LOGI(__VA_ARGS__); \
        }                                   \
    } while(false)

class AndroidEGLContent : public EGLContent
{
public:
    virtual void setup()
    {
        EGLBoolean res;
        EGLint majorVersion;
        EGLint minorVersion;

        _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        ERROR_ON_MSG(_display == EGL_NO_DISPLAY, "Failed to get display: 0x%x.", eglGetError());

        res = eglInitialize(_display, &majorVersion, &minorVersion);

        ERROR_ON_MSG(res == EGL_FALSE, "Failed to initialize egl: 0x%x.", eglGetError());

        const char *egl_extension_st = eglQueryString(_display, EGL_EXTENSIONS);
        ERROR_ON_MSG((strstr(egl_extension_st, "EGL_KHR_create_context") == nullptr), "Failed to query EGL_KHR_create_context");
        ERROR_ON_MSG((strstr(egl_extension_st, "EGL_KHR_surfaceless_context") == nullptr), "Failed to query EGL_KHR_surfaceless_context");

        const EGLint config_attribs[] =
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
            EGL_ALPHA_SIZE,     EGL_DONT_CARE,
            EGL_DEPTH_SIZE,     EGL_DONT_CARE,
            EGL_STENCIL_SIZE,   EGL_DONT_CARE,
            EGL_NONE
        };

        EGLConfig cfg;
        EGLint    count;

        res = eglChooseConfig(_display, config_attribs, &cfg, 1, &count);

        ERROR_ON_MSG(res == EGL_FALSE, "Failed to choose config: 0x%x.", eglGetError());
        res = eglBindAPI(EGL_OPENGL_ES_API);
        ERROR_ON_MSG(res == EGL_FALSE, "Failed to bind api: 0x%x.", eglGetError());

        const EGLint attribs[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
            EGL_CONTEXT_MINOR_VERSION_KHR, 1,
            EGL_NONE
        };
        _context = eglCreateContext(_display,
            cfg,
            EGL_NO_CONTEXT,
            attribs);

        ERROR_ON_MSG(_context == EGL_NO_CONTEXT, "Failed to create context: 0x%x.", eglGetError());
    }

    virtual void bindContext()
    {
        EGLBoolean res = eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, _context);
        ERROR_ON_MSG(res == EGL_FALSE, "Failed to make current: 0x%x.", eglGetError());
    }

    virtual void unbindContext()
    {
        EGLBoolean res = eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        ERROR_ON_MSG(res == EGL_FALSE, "Failed to make current: 0x%x.", eglGetError());
    }

    virtual void swapBuffer()
    {
        
    }

    virtual void* getShareContext()
    {
        const EGLint config_attribs[] =
        {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
            // EGL_RED_SIZE,       5,
            // EGL_GREEN_SIZE,     6,
            // EGL_BLUE_SIZE,      5,
            // EGL_ALPHA_SIZE,     EGL_DONT_CARE,
            // EGL_DEPTH_SIZE,     EGL_DONT_CARE,
            // EGL_STENCIL_SIZE,   EGL_DONT_CARE,
            EGL_NONE
        };
        EGLContext m_eglCurrentContext = eglGetCurrentContext();
        EGLDisplay m_eglCurrentDisplay = eglGetCurrentDisplay();
        // EGLConfig elgConfigTmp;
        // EGLint num_config;
        //eglGetConfigs(m_eglCurrentDisplay, &elgConfigTmp,  1, &num_config);
        EGLConfig cfg;
        EGLint    count;

        EGLBoolean res = eglChooseConfig(m_eglCurrentDisplay, config_attribs, &cfg, 1, &count);

        //  ERROR_ON_MSG(res == EGL_FALSE, "Failed to choose config: 0x%x.", eglGetError());
         // res = eglBindAPI(EGL_OPENGL_ES_API);
        //  ERROR_ON_MSG(res == EGL_FALSE, "Failed to bind api: 0x%x.", eglGetError());

        const EGLint attribs[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
            EGL_CONTEXT_MINOR_VERSION_KHR, 1,
            EGL_NONE
        };

        return (void *)eglCreateContext(m_eglCurrentDisplay,
            cfg,
            m_eglCurrentContext,
            attribs);
    }
    virtual void destory()
    {
        eglDestroyContext(_display, _context);
    } 

private:
    EGLDisplay _display;
    EGLContext _context;
};


EGLContent* EGLContentFactory::createEGLContentForAndroid()
{
    return new AndroidEGLContent();
}

#else

//#include <glad/glad.h>
#include <GLFW/glfw3.h>
class WindowsEGLContent : public EGLContent
{
public:
    virtual void setup()
    {
        glfwInit();
        // glfwWindowHint(GLFW_RED_BITS, 8);
        // glfwWindowHint(GLFW_GREEN_BITS, 8);
        // glfwWindowHint(GLFW_BLUE_BITS, 8);
        // glfwWindowHint(GLFW_ALPHA_BITS, 8);
        // glfwWindowHint(GLFW_DEPTH_BITS, 16);
        // glfwWindowHint(GLFW_STENCIL_BITS, 8);
        // glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        _window = glfwCreateWindow(512, 512, "", NULL, NULL);
        glfwMakeContextCurrent(_window);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    }

    virtual void bindContext()
    {
        glfwMakeContextCurrent(_window);
    }

    virtual void unbindContext()
    {
		glfwMakeContextCurrent(NULL);
    }

    virtual void swapBuffer()
    {
        glfwSwapBuffers(_window);
    }

    virtual void destory()
    {
		glfwDestroyWindow(_window);
		glfwTerminate();
    }
    virtual void* getShareContext()
    {
        return nullptr;
    }
private:
    GLFWwindow *_window;
};

EGLContent* EGLContentFactory::createEGLContentForWindows()
{
    return new WindowsEGLContent();
}
#endif
