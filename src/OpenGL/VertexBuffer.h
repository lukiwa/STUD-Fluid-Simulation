#include <GL/glew.h>


#pragma once
class VertexBuffer {
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
private:
    GLuint _id;
};

