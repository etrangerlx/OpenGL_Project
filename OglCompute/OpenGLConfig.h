#ifndef __OPENGL_CONFIG_H__
#define __OPENGL_CONFIG_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#else


#include <glad/glad.h>
//#define GLEW_STATIC
//#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#endif

#ifndef LOGI
#ifdef ANDROID
#include <stdio.h>
#include <android/log.h>
#define  LOG_TAG    "BlackSesame"
#define  LOGI(...) \
{ \
	{ \
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
	} \
} \

#else
#define  LOGI(...)  \
{ \
	printf(__VA_ARGS__); \
	printf("\n"); \
}

#endif
#endif

#endif