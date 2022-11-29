cmake_minimum_required(VERSION 3.10)
project(OpenGL_test)

set(GLAD_HOME  "E:/SourceCode/glad")
set(GLEW_HOME  "E:/SourceCode/GLEW")
set(GLFW_HOME  "E:/SourceCode/GLFW")
set(SOIL_HOME  "E:/SourceCode/SOIL2/build/Install")
set(GLM_HOME "E:/SourceCode/glm-0.9.9.8")
set(STB_HOME "E:/SourceCode/stb")

#find_library(
#        GLEW_LIBRARY
#        REQUIRED
#        HINTS  ${GLEW_HOME}/lib/cmake/glew
#)
#
#if(GLEW_FOUND)
#    message("Find GLEW: ${GLEW_INCLUDE_DIR}; ${GLEW_LIBRARY};")
#endif()
#include(${GLEW_HOME}/lib/cmake/glew/glew-config.cmake)

include_directories(${GLAD_HOME}/include)
include_directories(${GLEW_HOME}/include)
include_directories(${GLFW_HOME}/include)
include_directories(${SOIL_HOME}/include)
include_directories(${GLM_HOME})
include_directories(${STB_HOME})

link_directories(${GLEW_HOME}/lib)
link_directories(${GLFW_HOME}/lib)
link_directories(${SOIL_HOME}/lib)

set(CMAKE_CXX_STANDARD 14)
#设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#设置动态链接库文件到运行时目录
#设置资源文件到运行时目录
set(THRIFT_SRCS
        ${GLEW_HOME}/bin/*.dll
        ${GLFW_HOME}/bin/*.dll
        ${SOIL_HOME}/bin/*.dll
        ${CMAKE_CURRENT_LIST_DIR}/img/*
        ${CMAKE_CURRENT_LIST_DIR}/*.frag
        ${CMAKE_CURRENT_LIST_DIR}/*.vs)
message(STATUS ${THRIFT_SRCS})
file(GLOB_RECURSE DLL_FILE ${THRIFT_SRCS})
message(STATUS DLL_FILE = ${DLL_FILE})

file(COPY ${DLL_FILE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(OpenGL_test Shader.cpp main.cpp ${GLAD_HOME}/src/glad.c)

target_link_libraries(OpenGL_test   libglew32.dll.a libglfw3dll.a libsoil2.dll.a  ${GLFW_HOME}/lib opengl32)



