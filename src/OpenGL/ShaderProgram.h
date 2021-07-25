#pragma once
#include "string"
#include <GL/glew.h>


class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram() = default;

    ShaderProgram& VertexShaderFilename(const std::string& vertexShaderFilename);
    ShaderProgram& FragmentShaderFilename(const std::string& fragmentShaderFilename);
    ShaderProgram& CompileShaders();
    const ShaderProgram& LinkAndValidate() const;
    void Use() const;
    void Delete() const;


private:
    static std::string FileToString(const std::string& filepath);
    static GLuint CompileShader(GLuint type, const std::string& source);

    const std::string _shadersRoot;
    GLuint _programId{};
    GLuint _vertexShaderId{};
    GLuint _fragmentShaderId{};
    std::string _vertexShaderSource;
    std::string _fragmentShaderSource;
};

