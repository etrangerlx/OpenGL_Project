cmake_minimum_required(VERSION 3.19)
project(OpenGL_test)

set(GLAD_HOME  "E:/SourceCode/glad")
set(GLEW_HOME  "E:/SourceCode/GLEW")
set(GLFW_HOME  "E:/SourceCode/GLFW")
set(SOIL_HOME  "E:/SourceCode/SOIL2/build/Install")

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

link_directories(${GLEW_HOME}/lib)
link_directories(${GLFW_HOME}/lib)
link_directories(${SOIL_HOME}/lib)

set(CMAKE_CXX_STANDARD 14)

add_executable(OpenGL_test Shader.cpp main.cpp)

target_link_libraries(OpenGL_test   libglew32.dll.a libglfw3dll.a libsoil2.dll.a  ${GLFW_HOME}/lib opengl32)
#target_link_libraries(OpenGL_test glfw3 GLEW GL pthread)