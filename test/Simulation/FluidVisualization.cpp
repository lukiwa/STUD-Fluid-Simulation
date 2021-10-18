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

class MockPixelMap : public IPixelMap {
public:
    MockPixelMap(Dimensions dimensions, int internalPixelFormat)
        : _pixelType(internalPixelFormat)
        , _width(dimensions.x)
        , _height(dimensions.y)
    {
        _pixelBuffer.resize(_width * _height * PixelBufferObject::ConvertFormatToNumber(_pixelType));
    }

    void SetAllPixels(const std::vector<int>& components) override
    {
        for (int x = 0; x < _width; ++x) {
            for (int y = 0; y < _height; ++y) {
                SetPixel(x, y, components);
            }
        }
    }
    void SetPixel(int x, int y, const std::vector<int>& components) override
    {
        assert(x <= _width);
        assert(y <= _height);

        int pixelDepth = PixelBufferObject::ConvertFormatToNumber(_pixelType);

        assert(static_cast<std::size_t>(pixelDepth) == components.size());

        for (int i = 0; i < pixelDepth; ++i) {
            _pixelBuffer[pixelDepth * (x + y * _width) + i] = components[i];
        }
    }
    std::vector<GLubyte> GetPixel(int x, int y) const override
    {
        assert(x <= _width);
        assert(y <= _height);

        std::vector<GLubyte> result;
        int pixelDepth = PixelBufferObject::ConvertFormatToNumber(_pixelType);
        result.reserve(pixelDepth);

        for (int i = 0; i < pixelDepth; ++i) {
            result.push_back(_pixelBuffer[pixelDepth * (x + y * _width) + i]);
        }

        return result;
    }
    void SwapBuffer() override
    {
        // UNUSED
    }
    void Clear() override
    {
        SetAllPixels({ 255, 255, 255, 255 });
    }
    int GetWidth() const override
    {
        return _width;
    }
    int GetHeight() const override
    {
        return _height;
    }

private:
    int _pixelType;
    int _width;
    int _height;
    std::vector<GLubyte> _pixelBuffer;
};

class FluidVisualizationTestFixture : public ::testing::Test {
public:
    FluidVisualizationTestFixture()
        : _size(50)
        , _pixelMap(nullptr)
        , _visualization(nullptr)
    {
    }

    void SetUp() override
    {
        _pixelMap.reset(new MockPixelMap({ 50, 50, 0 }, GL_RGBA));
        _pixelMap->Clear();

        FluidVisualizationBuilder builder;
        builder.PixelMatrix(_pixelMap.get());
        _visualization = builder.Build();
    }

    void TearDown() override
    {
        _visualization.reset(nullptr);
    }

    int _size;
    std::unique_ptr<IPixelMap> _pixelMap;
    std::unique_ptr<IFluidVisualization> _visualization;
};

TEST_F(FluidVisualizationTestFixture, TheMoreDensityTheDarkerPixel)
{
    const int xChanged = 10;
    const int yChanged = 10;
    const int amount = 250;

    Matrix densityMap(_size);
    densityMap(xChanged, yChanged) = amount;

    _visualization->Update(densityMap);

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

    Matrix densityMap(_size);
    densityMap(xChanged, yChanged) = amount;
    _visualization->Update(densityMap);

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

    Matrix densityMap(_size);
    densityMap(xChanged, yChanged) = amount;
    _visualization->Update(densityMap);

    auto pixel = _pixelMap->GetPixel(xChanged, yChanged);
    for (const auto& component : pixel) {
        ASSERT_LE(component, 255);
    }
}
