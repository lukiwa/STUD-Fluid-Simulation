#include <GL/glew.h>


#pragma once
class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;
private:
    GLuint _id;
};