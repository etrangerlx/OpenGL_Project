//
// Created by Administrator on 2023/11/8.
//

#ifndef OGL_SHADER_H
#define OGL_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

#include "OpenGLConfigure.h"

#include "GLSL.h"

class Shader {
public:
// 程序ID
    GLuint Program;
    GLuint vertex;
    GLuint fragment;

// 构造器读取并创建Shader
    Shader();

    Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath);

    Shader(const char *vertex, const char *fragment,bool isfile);

    ~Shader();

// 使用Program
    void Use();
};


#endif //OGL_SHADER_H
