#include "PixelMap.h"
#include <iostream>
#include <chrono>

/**
 * Create new pixelmap
 * @param width
 * @param height
 * @param type pixeltype (eg. GL_RGBA)
 */
PixelMap::PixelMap(int width, int height, int type)
    : _pixelType(type)
    , _width(width)
    , _height(height)
    , _pbo(width, height, type)
    , _type(type)
{
    _texture = Texture::Factory::Create(GL_TEXTURE_2D, width, height, 0, type, GL_UNSIGNED_BYTE, nullptr);
}

/**
 * Bind pixelmap
 */
void PixelMap::Bind() const
{
    _pbo.Bind();
    _texture->Bind();
}
/*
 * Unbind pixelmap
 */
void PixelMap::Unbind() const
{
    _pbo.Unbind();
    _texture->Unbind();
}

/**
 * Set given pixel to components value
 * NOTE: components size is not checked
 * @param x x coord
 * @param y y coord
 * @param components pixel components (eg. RGBA values)
 */
void PixelMap::SetPixel(int x, int y, const std::vector<int>& components)
{
    Bind();
    assert(x <= _width);
    assert(y <= _height);

    auto* pixels = static_cast<GLubyte*>(_pbo.MapBuffer());
    int pixelDepth = PixelBufferObject::ConvertFormatToNumber(_pixelType);

    assert(pixelDepth == components.size());

    for (int i = 0; i < pixelDepth; ++i) {
        pixels[pixelDepth * (x + y * _width) + i] = components[i];
    }

    _pbo.UnmapBuffer();

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

/**
 * Set all pixels to component values
 * @param components components values
 */


void PixelMap::SetAllPixels(const std::vector<int>& components)
{
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            SetPixel(x, y, components);
        }
    }
}
void PixelMap::Clear() const
{
    Bind();

    auto* pixels = static_cast<GLubyte*>(_pbo.MapBuffer());
    std::fill_n(pixels, _width * _height * PixelBufferObject::ConvertFormatToNumber(_pixelType), 255);
    _pbo.UnmapBuffer();

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}