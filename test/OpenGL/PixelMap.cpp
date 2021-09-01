#include "Tracing.h"
#include "Window.h"
#include "gtest/gtest.h"
#include <GL/glew.h>
#include <gmock/gmock.h>

#include "PixelBufferObject.h"
#include "PixelMap.h"
#include "Texture.h"

class MockPixelBufferObject : public IPixelBufferObject {
public:
    MockPixelBufferObject(int width, int height, int internalFormat)
    {
        _buffer.resize(width * height * internalFormat);
    }
    void Bind() const override
    {
        // UNUSED
    }
    void Unbind() const override
    {
        // UNUSED
    }
    void* MapBuffer() const override
    {
        return (void*)(_buffer.data());
    }
    void UnmapBuffer() const override
    {
        _buffer.clear();
    }

private:
    mutable std::vector<GLubyte> _buffer;
};

class MockTexture : public ITexture {
public:
    void Bind() const override
    {
        // UNUSED
    }
    void Unbind() const override
    {
        // UNUSED
    }
    void SetTextureParameters() const override
    {
        // UNUSED
    }
    void SubImage(void*) const override
    {
        // UNUSED
    }
};

class MockComponentFactory : public IPixelMapComponentsFactory {
public:
    Components CreateComponents(Dimensions dimensions, int internalPixelFormat, GLenum, const void*) const override
    {
        std::unique_ptr<IPixelBufferObject> pbo(
            new MockPixelBufferObject(dimensions.x, dimensions.y, internalPixelFormat));
        std::unique_ptr<ITexture> texture(new MockTexture());
        texture->SetTextureParameters();
        return { std::move(pbo), std::move(texture) };
    }
};

TEST(PixelMapTests, RealImplementationReturnsNonNullptrMappedBuffer)
{
    ALLOW_DISPLAY;
    const int width = 255;
    const int height = 255;
    const GLenum pixelFormat = GL_RGBA;

    std::unique_ptr<GLFW::Window> _window;
    ASSERT_NO_THROW(_window.reset(new GLFW::Window(10, 10, "Test")));
    ASSERT_EQ(glewInit(), GLEW_OK);

    PixelBufferObject pbo(width, height, pixelFormat);
    pbo.Bind();
    ASSERT_NE(pbo.MapBuffer(), nullptr);
}

TEST(PixelMapTests, MockPixelInternalFormatMatchesRGBA)
{
    const int width = 255;
    const int height = 255;
    const GLenum pixelFormat = GL_RGBA;
    const int pixelFormatNumber = PixelBufferObject::ConvertFormatToNumber(pixelFormat);

    std::unique_ptr<IPixelMapComponentsFactory> factory(new MockComponentFactory);
    PixelMap pixelMap({ width, height, 0 }, pixelFormat, factory.get());

    pixelMap.SetPixel(1, 1, { 255, 255, 255, 255 });
    auto pixel = pixelMap.GetPixel(1, 1);

    ASSERT_EQ(pixel.size(), pixelFormatNumber);
}

TEST(PixelMapTests, MockPixelInternalFormatMatchesRGB)
{
    const int width = 255;
    const int height = 255;
    const GLenum pixelFormat = GL_RGB;
    const int pixelFormatNumber = PixelBufferObject::ConvertFormatToNumber(pixelFormat);

    std::unique_ptr<IPixelMapComponentsFactory> factory(new MockComponentFactory);
    PixelMap pixelMap({ width, height, 0 }, pixelFormat, factory.get());

    pixelMap.SetPixel(1, 1, { 255, 255, 255 });
    auto pixel = pixelMap.GetPixel(1, 1);

    ASSERT_EQ(pixel.size(), pixelFormatNumber);
}

TEST(PixelMapTests, MockPixelComponentsMatches)
{
    const int width = 255;
    const int height = 255;
    const GLenum pixelFormat = GL_RGBA;
    const int pixelFormatNumber = PixelBufferObject::ConvertFormatToNumber(pixelFormat);

    std::unique_ptr<IPixelMapComponentsFactory> factory(new MockComponentFactory);
    PixelMap pixelMap({ width, height, 0 }, pixelFormat, factory.get());

    pixelMap.SetPixel(1, 1, { 100, 100, 100, 100 });
    auto pixel = pixelMap.GetPixel(1, 1);

    ASSERT_EQ(pixel.size(), pixelFormatNumber);
    for (const auto& component : pixel) {
        ASSERT_EQ(component, 100);
    }
}

TEST(PixelMapTests, MockPixelsChangeOnlyOnGivenRange)
{
    const int width = 100;
    const int height = 100;

    const int widthRangeToChange = width / 2;
    const int heightRangeToChange = height / 2;

    const GLenum pixelFormat = GL_RGBA;

    std::unique_ptr<IPixelMapComponentsFactory> factory(new MockComponentFactory);
    PixelMap pixelMap({ width, height, 0 }, pixelFormat, factory.get());
    pixelMap.Clear();

    for (int x = 0; x < widthRangeToChange; ++x) {
        for (int y = 0; y < heightRangeToChange; ++y) {
            pixelMap.SetPixel(x, y, { 100, 100, 100, 100 });

            auto pixel = pixelMap.GetPixel(x, y);
            for (const auto& component : pixel) {
                ASSERT_EQ(component, 100);
            }
        }
    }

    for (int x = widthRangeToChange; x < width; ++x) {
        for (int y = heightRangeToChange; y < height; ++y) {
            auto pixel = pixelMap.GetPixel(x, y);
            for (const auto& component : pixel) {
                ASSERT_EQ(component, 255);
            }
        }
    }
}
