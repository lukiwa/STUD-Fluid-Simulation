#include <GL/glew.h>
#include <memory>

#pragma once

class Texture {
private:
    explicit Texture(GLenum target);

    void SetTextureParameters() const;

public:
    ~Texture();

    class Factory {
    public:
        static std::unique_ptr<Texture> Create(GLenum textureType, int width, int height, int depth, GLenum pixelFormat,
            GLenum dataType, const void* data);
    };

    void Bind() const;
    void Unbind() const;

private:
    GLenum _target;
    GLuint _id;
};