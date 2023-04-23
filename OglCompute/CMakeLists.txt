cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME  OglCompute)
project(${PROJECT_NAME})
set(GLAD_HOME  "E:/SourceCode/glad")
set(GLM_HOME "E:/SourceCode/glm-0.9.9.8")
#set(GLEW_HOME  "E:/SourceCode/GLEW")
set(GLFW_HOME  "E:/SourceCode/GLFW")
include_directories(${GLAD_HOME}/include)
include_directories(${GLM_HOME})
#include_directories(${GLEW_HOME}/include)
include_directories(${GLFW_HOME}/include)
set(SRC_FILES
    Define.hpp
    EGLContent.hpp 
    EGLContent.cpp 
    OglCommonTexture.cpp 
    OglHardwareTexture.cpp 
    OglTexture.hpp
    FrameBufferObject.hpp 
    FrameBufferObject.cpp 
    ImageScaleRenderer.cpp
    ImageScaleRenderer.hpp
    Renderer.hpp 
    Renderer.cpp 
    ImageMatrixRenderer.cpp
    ImageMatrixRenderer.hpp
    ImageResampleRenderer.cpp
    ImageResampleRenderer.hpp
    ImageNV21Renderer.cpp
    ImageNV21Renderer.hpp
    LogoRenderer.cpp
    LogoRenderer.h
    OglCommonSSBO.cpp
    OglBaseMem.cpp
    OglBaseMem.hpp
	ImageScaleRGB2RGBH.cpp
	ImageScaleRGB2RGBH.hpp
	MutiExposureRender.cpp
	MutiExposureRender.hpp
	GridFilterRender.cpp
	GridFilterRender.hpp
	OglGraphicTexture.cpp
	OglGraphicTexture.h
)


set(SRC_FILES
	${SRC_FILES}
	${GLAD_HOME}/src/glad.c
)
message(STATUS ${SRC_FILES})
add_library(${PROJECT_NAME} ${SRC_FILES})

