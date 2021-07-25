#include "ShaderProgram.h"
#include "../Tracing.h"
#include <fstream>
#include <iostream>
#include <sstream>

// constructors
ShaderProgram::ShaderProgram()
    : _shadersRoot("../src/OpenGL/shaders/")
{
}

// public methods
ShaderProgram& ShaderProgram::VertexShaderFilename(const std::string& vertexShaderFilename)
{
    _vertexShaderSource = FileToString(_shadersRoot + vertexShaderFilename);
    return *this;
}

ShaderProgram& ShaderProgram::FragmentShaderFilename(const std::string& fragmentShaderFilename)
{
    _fragmentShaderSource = FileToString(_shadersRoot + fragmentShaderFilename);
    return *this;
}

ShaderProgram& ShaderProgram::CompileShaders()
{
    _programId = glCreateProgram();
    if (!_vertexShaderSource.empty() && !_fragmentShaderSource.empty()) {
        _vertexShaderId = CompileShader(GL_VERTEX_SHADER, _vertexShaderSource);
        _fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, _fragmentShaderSource);
    } else {
        LOG_ERROR("ONE OF THE SHADER SOURCE IS EMPTY!");
    }

    return *this;
}

const ShaderProgram& ShaderProgram::LinkAndValidate() const
{
    GlAssert(glAttachShader(_programId, _vertexShaderId));
    GlAssert(glAttachShader(_programId, _fragmentShaderId));
    GlAssert(glLinkProgram(_programId));
    GlAssert(glDeleteShader(_vertexShaderId));
    GlAssert(glDeleteShader(_fragmentShaderId));

    int result;
    GlAssert(glGetProgramiv(_programId, GL_LINK_STATUS, &result));
    if (result == GL_FALSE) {
        char message[512];
        GlAssert(glGetProgramInfoLog(_programId, sizeof(message), nullptr, message));
        LOG_ERROR("FAILED TO LINK SHADERS TO PROGRAM. MESSAGE: \n %s \n", message);
    }

    GlAssert(glDeleteShader(_vertexShaderId));
    GlAssert(glDeleteShader(_fragmentShaderId));
    return *this;
}
void ShaderProgram::Use() const { GlAssert(glUseProgram(_programId)); }
void ShaderProgram::Delete() const { GlAssert(glDeleteProgram(_programId)); }

// private methods
std::string ShaderProgram::FileToString(const std::string& filepath)
{
    {
        std::ifstream file(filepath);
        std::ostringstream oss;
        if (file.is_open()) {
            oss << file.rdbuf();
        } else {
            LOG_ERROR("ERROR OPENING FILE");
        }
        return oss.str();
    }
}
GLuint ShaderProgram::CompileShader(GLuint type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src_c_str = source.c_str();
    GlAssert(glShaderSource(id, 1, &src_c_str, nullptr));
    GlAssert(glCompileShader(id));

    int result;
    GlAssert(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        char message[1024];

        GlAssert(glGetShaderInfoLog(id, sizeof(message), nullptr, message));
        LOG_ERROR("FAILED TO COMPILE SHADER OF TYPE: %d. MESSAGE: \n %s \n", type, message);
        GlAssert(glDeleteShader(id));
        id = 0;
    }

    return id;
}
