#ifndef __DEFINE_HPP__
#define __DEFINE_HPP__

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <thread>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#else
#include <glad/glad.h>
//#define GLEW_STATIC
//#include <GL/glew.h>
#endif

#ifndef LOGI

#ifdef ANDROID
#include <android/log.h>
#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "OpenglesCompute", "[%s : %d] " fmt, __FUNCTION__, __LINE__,##__VA_ARGS__)
#else
#define LOGI(...)    do{printf("[%s : %d] : ", __FUNCTION__, __LINE__);printf(__VA_ARGS__); printf("\n");}while(0)
#endif

#endif

#define DEBUG__PRINT
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


#define PRINT {\
    printf("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__);\
    LOGI("-----LINE = %d FUNC = %s \n",__LINE__,__FUNCTION__); \
    CHECK;\
}
#else
#define PRINT 
#define CHECK
#endif

#endif

