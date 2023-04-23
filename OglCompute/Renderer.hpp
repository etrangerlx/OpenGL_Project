#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <iostream>

#include "OpenGLConfig.h"
#include "Define.hpp"
class Shader
{
public:
    Shader();

    ~Shader();
    
    bool compile();

    bool compileCS();

    void use();

    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

	void setFloatArray(const std::string &name, int size, float* value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;

    void setVec2(const std::string &name, float x, float y) const;

	void setVec2Array(const std::string &name, int size, float* value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;

    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;

    void setMat3(const std::string &name, const glm::mat3 &mat) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;


protected:
    GLuint m_pid;

    std::string m_vs;
    std::string m_fs;
    std::string m_cs;
};


class Renderer
{
public:
    Renderer() 
        : m_vao(GL_INVALID_VALUE)
        , m_vbo(GL_INVALID_VALUE)
        , m_ebo(GL_INVALID_VALUE)
    {}

    virtual ~Renderer() {}

    virtual bool init() = 0;

    virtual bool initUseCS() { return true; };

    virtual void render() = 0;

    virtual void renderUseCS() { return; };

    virtual void deinit() = 0;

    virtual void checkGLError();

protected:
    void initVao();

    void deinitVao();
    GLuint      m_vao;
    GLuint      m_vbo;
    GLuint      m_ebo;
};
#endif