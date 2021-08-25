#pragma once
#include "Dimensions.h"
#include "IBoundable.h"
#include "PixelBufferObject.h"
#include "Texture.h"
#include <cassert>
#include <vector>

class IPixelMapComponentsFactory {
public:
    using Components = std::pair<std::unique_ptr<IPixelBufferObject>, std::unique_ptr<ITexture>>;
    virtual ~IPixelMapComponentsFactory() = default;
    virtual Components CreateComponents(
        Dimensions dimensions, int internalPixelFormat, GLenum dataType, const void* data) const = 0;
};

class PixelMapComponentsFactory : public IPixelMapComponentsFactory {
public:
    virtual Components CreateComponents(
        Dimensions dimensions, int internalPixelFormat, GLenum dataType, const void* data) const override
    {
        std::unique_ptr<IPixelBufferObject> pbo(new PixelBufferObject(dimensions.x, dimensions.y, internalPixelFormat));
        std::unique_ptr<ITexture> texture(new Texture(
            dimensions.z == 0 ? GL_TEXTURE_2D : GL_TEXTURE_3D, dimensions, internalPixelFormat, dataType, data));
        texture->SetTextureParameters();
        return { std::move(pbo), std::move(texture) };
    }
};

class PixelMap : public IBoundable {
public:
    void Bind() const override;
    void Unbind() const override;

    PixelMap(Dimensions dimensions, int internalPixelFormat, IPixelMapComponentsFactory* factory);
    ~PixelMap() = default;

    void SetAllPixels(const std::vector<int>& components);
    void SetPixel(int x, int y, const std::vector<int>& components);
    void SwapBuffer();

    void Clear();

private:
    int _pixelType;
    int _width;
    int _height;

    std::unique_ptr<IPixelBufferObject> _pbo;
    std::unique_ptr<ITexture> _texture;
    std::vector<GLubyte> _pixelBuffer;
};
