#include "Texture.h"
#include "Tracing.h"

/**
 * Constructs new texture
 * @param target type of texture (eg. GL_TEXTURE_2D)
 */
Texture::Texture(GLenum target)
    : _id(0)
{
    _target = target;
    GlAssert(glGenTextures(1, &_id));
}
/**
 * Destructs the texture and deletes it
 */
Texture::~Texture()
{
    glDeleteTextures(1, &_id);
}

/**
 * Set texture parameters
 */
void Texture::SetTextureParameters() const
{
    GlAssert(glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GlAssert(glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GlAssert(glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GlAssert(glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

/**
 * Bind texture
 */
void Texture::Bind() const
{
    GlAssert(glBindTexture(_target, _id));
}

/**
 * Unbind texture
 */
void Texture::Unbind() const
{
    GlAssert(glBindTexture(_target, 0));
}

/**
 * Create new texture
 * @param textureType type of texture (eg. GL_TEXTURE_2D)
 * @param width texture width
 * @param height texture height
 * @param depth texture depth (0 in contex of 2d texture)
 * @param pixelFormat pixel format (eg. GL_RGBA)
 * @param dataType pixel type (eg. GL_UNSIGNED_BYTE)
 * @param data texture data
 * @return new texture
 */
std::unique_ptr<Texture> Texture::Factory::Create(
    GLenum textureType, int width, int height, int depth, GLenum pixelFormat, GLenum dataType, const void* data)
{

    std::unique_ptr<Texture> texture(new Texture(textureType));
    texture->Bind();
    texture->SetTextureParameters();
    GLenum formatConverted = pixelFormat == GL_BGRA || GL_RGBA ? GL_RGBA : pixelFormat;

    switch (textureType) {
    case GL_TEXTURE_2D:
        GlAssert(glTexImage2D(GL_TEXTURE_2D, 0, formatConverted, width, height, 0, pixelFormat, dataType, data));
        break;
    case GL_TEXTURE_3D:
        GlAssert(glTexImage3D(GL_TEXTURE_3D, 0, formatConverted, width, height, depth, 0, pixelFormat, dataType, data));
        break;
    default:
        LOG_ERROR("UNKNOWN TEXTURE TYPE!");
        break;
    }
    return texture;
}
