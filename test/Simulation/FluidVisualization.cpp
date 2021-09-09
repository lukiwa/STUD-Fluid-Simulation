#include "Tracing.h"
#include "Window.h"
#include "gtest/gtest.h"
#include <GL/glew.h>
#include <gmock/gmock.h>

#include "PixelBufferObject.h"
#include "PixelMap.h"
#include "Texture.h"

#include "FluidBuilder.h"
#include "PixelMap.h"

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

class FluidVisualizationTestFixture : public ::testing::Test {
public:
    FluidVisualizationTestFixture()
    {
    }

    void SetUp() override
    {
        _size = 50;
        _pixelMap.reset(new PixelMap({ 50, 50, 0 }, GL_RGBA, new MockComponentFactory()));
        _pixelMap->Clear();

        _fluidBuilder.reset(new FluidBuilder());
        _fluidBuilder->Size({ _pixelMap->GetWidth(), _pixelMap->GetHeight(), 0 }).DyeMatrix(*_pixelMap);
        _fluid = _fluidBuilder->Build();
    }

    void TearDown() override
    {
    }

    int _size;
    std::unique_ptr<PixelMap> _pixelMap;
    std::unique_ptr<FluidBuilder> _fluidBuilder;
    std::unique_ptr<Fluid> _fluid;
};

TEST_F(FluidVisualizationTestFixture, TheMoreDensityTheDarkerPixel)
{
    const int xChanged = 10;
    const int yChanged = 10;
    const int amount = 250;

    _fluid->AddDensity(xChanged, yChanged, amount);
    _fluid->Step();

    for (int x = 0; x < _pixelMap->GetWidth(); ++x) {
        for (int y = 0; y < _pixelMap->GetHeight(); ++y) {

            auto pixel = _pixelMap->GetPixel(x, y);
            if (x == xChanged && y == yChanged) {
                for (const auto& component : pixel) {
                    ASSERT_EQ(component, 255 - amount);
                }
            } else {
                for (const auto& component : pixel) {
                    ASSERT_EQ(component, 255);
                }
            }
        }
    }
}

TEST_F(FluidVisualizationTestFixture, PixelBrightnessNotLessThanZeroIfLotsOfDensityAdded)
{
    const int xChanged = 10;
    const int yChanged = 10;
    const int amount = 512;

    _fluid->AddDensity(xChanged, yChanged, amount);
    _fluid->Step();
    auto pixel = _pixelMap->GetPixel(xChanged, yChanged);
    for (const auto& component : pixel) {
        ASSERT_GE(component, 0);
    }
}

TEST_F(FluidVisualizationTestFixture, PixelBrightnessNotMoreThan255IfSmallDensityAdded)
{
    const int xChanged = 10;
    const int yChanged = 10;
    const int amount = 0;

    _fluid->AddDensity(xChanged, yChanged, amount);
    _fluid->Step();
    auto pixel = _pixelMap->GetPixel(xChanged, yChanged);
    for (const auto& component : pixel) {
        ASSERT_LE(component, 255);
    }
}