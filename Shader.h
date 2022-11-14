//
// Created by Administrator on 2022/11/13.
//

#ifndef OPENGL_TEST_SHADERR_H
#define OPENGL_TEST_SHADERR_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
#include <GL/glew.h> // 包含glew获取所有的OpenGL必要headers


class Shader {
public:
// 程序ID
    GLuint Program;
// 构造器读取并创建Shader
    Shader(const GLchar * vertexSourcePath, const GLchar * fragmentSourcePath);
// 使用Program
    void Use();

};


#endif //OPENGL_TEST_SHADERR_H
