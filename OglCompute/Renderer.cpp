#include <vector>

#include "Renderer.hpp"

Shader::Shader()
	: m_pid(0)
{
    m_vs = 
        #ifdef OpenGLES 
            "#version 320 es\n\
            precision lowp float;\n"
        #else
            "#version 430\n"
        #endif
            "layout(location = 0) in vec3 in_vertex;\n\
            layout(location = 1) in vec2 in_texcoord;\n\
            out vec2 texcoord;\n\
            void main(){\n\
                gl_Position = vec4(in_vertex, 1);\n\
                texcoord = in_texcoord;\n\
            }";
	m_cs =
#ifdef OpenGLES 
		"#version 320 es\n\
         precision lowp float;\n"
#else
		"#version 430\n"
#endif
		"uniform float roll;\
		 uniform image2D tex0;\
		 layout (local_size_x = 16, local_size_y = 16) in;\
		 void main() {\
			 ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
			 float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);\
			 float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y)*0.1 + roll)*0.5;\
			 imageStore(tex0, storePos, vec4(1.0-globalCoef*localCoef, 0.0, 0.0, 0.0));\
		 }";
}

Shader::~Shader()
{
	if (m_pid != 0)
	{
		glDeleteProgram(m_pid);
		m_pid = 0;
	}
}

bool Shader::compile()
{
	std::string fs_header = 
            #ifdef OpenGLES 
                "#version 320 es\n"
                "#extension GL_OES_EGL_image_external_essl3 : enable \n"
            #else
                "#version 430\n"
            #endif
            ;
	m_vs = 
        #ifdef OpenGLES 
            "#version 320 es\n"
			"#extension GL_OES_EGL_image_external_essl3 : enable \n"
            "precision highp float;\n"
        #else
            "#version 430\n"
        #endif
            "layout(location = 0) in vec3 in_vertex;\n\
            layout(location = 1) in vec2 in_texcoord;\n\
            out vec2 texcoord;\n\
            void main(){\n\
                gl_Position = vec4(in_vertex, 1);\n\
                texcoord = in_texcoord;\n\
            }";
	GLuint vsId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsId = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int infoLength;

	// Compile Vertex Shader
	char const * vsp = (m_vs).c_str();
	glShaderSource(vsId, 1, &vsp, NULL);
	glCompileShader(vsId);

	// Check Vertex Shader
	glGetShaderiv(vsId, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderiv(vsId, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength > 0) {
			std::vector<char> vsErrMsg(infoLength + 1);
			glGetShaderInfoLog(vsId, infoLength, NULL, &vsErrMsg[0]);
			LOGI("%s\n", &vsErrMsg[0]);
            printf("%s\n", &vsErrMsg[0]);
	    }
		return false;
    }

    // Compile Fragment Shader
    std::string fragShader = fs_header+m_fs;
    char const * fsp = fragShader.c_str();
    glShaderSource(fsId, 1, &fsp, NULL);
    glCompileShader(fsId);

	// Check Fragment Shader
	glGetShaderiv(fsId, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderiv(fsId, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength > 0) {
			std::vector<char> fsErrMsg(infoLength + 1);
			glGetShaderInfoLog(fsId, infoLength, NULL, &fsErrMsg[0]);
			LOGI("%s\n", &fsErrMsg[0]);
            printf("%s\n", &fsErrMsg[0]);
		}
		return false;
	}

	// Link the program
	m_pid = glCreateProgram();
	glAttachShader(m_pid, vsId);
	glAttachShader(m_pid, fsId);
	glLinkProgram(m_pid);

	// Check the program
	glGetProgramiv(m_pid, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetProgramiv(m_pid, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength > 0) {
			std::vector<char> ProgramErrorMessage(infoLength + 1);
			glGetProgramInfoLog(m_pid, infoLength, NULL, &ProgramErrorMessage[0]);
			LOGI("%s\n", &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
		}
		return false;
	}

	glDetachShader(m_pid, vsId);
	glDetachShader(m_pid, fsId);

	glDeleteShader(vsId);
	glDeleteShader(fsId);

	return true;
}

bool Shader::compileCS()
{
	GLuint csId = glCreateShader(GL_COMPUTE_SHADER);

	GLint Result = GL_FALSE;
	int infoLength;

	// Compile Vertex Shader
	char const* csp = (m_cs).c_str();
	glShaderSource(csId, 1, &csp, NULL);
	glCompileShader(csId);

	// Check Vertex Shader
	glGetShaderiv(csId, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderiv(csId, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength > 0) {
			std::vector<char> csErrMsg(infoLength + 1);
			glGetShaderInfoLog(csId, infoLength, NULL, &csErrMsg[0]);
			LOGI("%s\n", &csErrMsg[0]);
			printf("%s\n", &csErrMsg[0]);
		}
		return false;
	}

	m_pid = glCreateProgram();
	glAttachShader(m_pid, csId);
	glLinkProgram(m_pid);

	glGetProgramiv(m_pid, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetProgramiv(m_pid, GL_INFO_LOG_LENGTH, &infoLength);
		if (infoLength > 0) {
			std::vector<char> ProgramErrorMessage(infoLength + 1);
			glGetProgramInfoLog(m_pid, infoLength, NULL, &ProgramErrorMessage[0]);
			LOGI("%s\n", &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}
		return false;
	}

	glDetachShader(m_pid, csId);
	glDeleteShader(csId);
	return true;
}

void Shader::use()
{
	glUseProgram(m_pid);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_pid, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_pid, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_pid, name.c_str()), value);
}

void Shader::setFloatArray(const std::string &name, int size, float* value) const
{
	glUniform1fv(glGetUniformLocation(m_pid, name.c_str()), size, value);

}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(m_pid, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_pid, name.c_str()), x, y);
}

void Shader::setVec2Array(const std::string &name, int size, float* value) const
{
	glUniform2fv(glGetUniformLocation(m_pid, name.c_str()), size, value);

}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(m_pid, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_pid, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(m_pid, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_pid, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_pid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_pid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_pid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Renderer::checkGLError()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        switch (error)
        {
        case GL_INVALID_ENUM:
			LOGI("GL Error: GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
			LOGI("GL Error: GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
			LOGI("GL Error: GL_INVALID_OPERATION");
            break;
#ifdef _WIN32
        case GL_STACK_OVERFLOW:
			LOGI("GL Error: GL_STACK_OVERFLOW");
            break;
        case GL_STACK_UNDERFLOW:
			LOGI("GL Error: GL_STACK_UNDERFLOW");
            break;
        case GL_OUT_OF_MEMORY:
			LOGI("GL Error: GL_OUT_OF_MEMORY");
            break;
#endif
        default:
			LOGI("GL Error: 0x%x \n", error);
            break;
        }
    }
}

void Renderer::initVao()
{
    float data[] = {
        -1.f,  1.f, 0.f,  	0.f, 1.f,
         1.f,  1.f, 0.f, 	1.f, 1.f,
        -1.f, -1.f, 0.f, 	0.f, 0.f,
         1.f, -1.f, 0.f, 	1.f, 0.f,
    };

    unsigned short indice[] = {
        0, 1, 2,
        2, 1, 3
    };
    
    glGenVertexArrays(1, &m_vao);CHECK;
    glGenBuffers(1, &m_vbo);CHECK;
    glGenBuffers(1, &m_ebo);CHECK;

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);CHECK;
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);CHECK;

    glBindVertexArray(m_vao);CHECK;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);CHECK;
    glEnableVertexAttribArray(0);CHECK;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);CHECK;
    
    glEnableVertexAttribArray(1);CHECK;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));CHECK;
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);CHECK;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);CHECK;

    glBindVertexArray(0);CHECK;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);CHECK;
    glBindBuffer(GL_ARRAY_BUFFER, 0);CHECK;
}

void Renderer::deinitVao()
{
    if (m_vao != GL_INVALID_VALUE)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = GL_INVALID_VALUE;
    }

    if (m_vbo != GL_INVALID_VALUE)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = GL_INVALID_VALUE;
    }

    if (m_ebo != GL_INVALID_VALUE)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = GL_INVALID_VALUE;
    }
}
