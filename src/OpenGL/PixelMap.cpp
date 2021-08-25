#include "PixelMap.h"
#include <chrono>
#include <iostream>

/**
 * Create new pixelmap
 * @param width
 * @param height
 * @param type pixeltype (eg. GL_RGBA)
 */
PixelMap::PixelMap(Dimensions dimensions, int internalPixelFormat, IPixelMapComponentsFactory* factory)
    : _pixelType(internalPixelFormat)
    , _width(dimensions.x)
    , _height(dimensions.y)
{
    auto components = factory->CreateComponents(dimensions, internalPixelFormat, GL_UNSIGNED_BYTE, nullptr);
    _pbo = std::move(components.first);
    _texture = std::move(components.second);
    _pixelBuffer.resize(_width * _height * PixelBufferObject::ConvertFormatToNumber(_pixelType));
}

/**
 * Bind pixelmap
 */
void PixelMap::Bind() const
{
    _pbo->Bind();
    _texture->Bind();
}
/*
 * Unbind pixelmap
 */
void PixelMap::Unbind() const
{
    _pbo->Unbind();
    _texture->Unbind();
}

void PixelMap::SwapBuffer()
{
    Bind();
    auto* pixels = static_cast<GLubyte*>(_pbo->MapBuffer());
    std::copy(_pixelBuffer.begin(), _pixelBuffer.end(), pixels);
    _pbo->UnmapBuffer();
    _texture->SubImage(nullptr);
}

/**
 * Set given pixel to components value
 * NOTE: components size is not checked
 * NOTE: You should use SwapBuffers afterwards
 * @param x x coord
 * @param y y coord
 * @param components pixel components (eg. RGBA values)
 */
void PixelMap::SetPixel(int x, int y, const std::vector<int>& components)
{
    assert(x <= _width);
    assert(y <= _height);

    int pixelDepth = PixelBufferObject::ConvertFormatToNumber(_pixelType);

    assert(static_cast<std::size_t>(pixelDepth) == components.size());

    for (int i = 0; i < pixelDepth; ++i) {
        _pixelBuffer[pixelDepth * (x + y * _width) + i] = components[i];
    }

}

/**
 * Set all pixels to component values
 * NOTE: You should use SwapBuffers afterwards
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

/**
 * Set all pixels to white
 */
void PixelMap::Clear()
{
    SetAllPixels({255, 255, 255, 255});
}

