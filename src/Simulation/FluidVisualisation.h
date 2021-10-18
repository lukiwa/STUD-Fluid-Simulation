#pragma once
#include "../Utilities/Matrix.h"
#include "../OpenGL/PixelMap.h"
#include "IFluid.h"


class FluidVisualization : public IFluidVisualization {
public:
    void Update(const Matrix& densityMap) override;

private:
    friend class FluidVisualizationBuilder;
    FluidVisualization(IPixelMap* pixelMap);

    IPixelMap* _pixelMap;

    uint8_t _singlePixelDyeAmount;

};