#pragma once
#include <GL/glew.h>
class PixelBufferObject {

public:
    PixelBufferObject(int width, int height, int internalFormat);
    ~PixelBufferObject();

    void Bind() const;
    void Unbind() const;

    void* MapBuffer();
    void UnmapBuffer();

    static int ConvertFormatToNumber(int internalFormat);

private:
    GLuint _id;
};
