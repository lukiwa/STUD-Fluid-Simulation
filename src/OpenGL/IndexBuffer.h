#include <GL/glew.h>

#pragma once
class IndexBuffer {
public:
    IndexBuffer(const GLuint* data, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    inline uint32_t Count() const { return _count; }

private:
    GLuint _id;
    uint32_t _count;
};
