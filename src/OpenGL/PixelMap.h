#pragma once
#include "PixelBufferObject.h"
#include "Texture.h"
#include <cassert>
#include <vector>

class PixelMap {
public:
    void Bind() const;
    void Unbind() const;

    PixelMap(int width, int height, int type);
    ~PixelMap() = default;

    void SetAllPixels(const std::vector<int>& components);

    void SetPixel(int x, int y, const std::vector<int>& components);

    void Clear() const;
private:
    int _pixelType;
    int _width;
    int _height;
    int _type;

    PixelBufferObject _pbo;
    std::unique_ptr<Texture> _texture;
};
