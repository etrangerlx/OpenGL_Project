//
// Created by Administrator on 2023/11/8.
//

#ifndef OGL_OPENGLCONFIGURE_H
#define OGL_OPENGLCONFIGURE_H
#ifdef ANDROID

#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#else

#ifdef __GLAD__

#include <glad/glad.h>

#elif __GLEW__

#define GLEW_STATIC

#include <GL/glew.h>

#endif
// GLFW
#include <GLFW/glfw3.h>

#endif
#endif //OGL_OPENGLCONFIGURE_H
