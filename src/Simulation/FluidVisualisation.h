#pragma once
#include "../Utilities/Matrix.h"
#include "../OpenGL/PixelMap.h"
#include "IFluid.h"


class FluidVisualization : public IFluidVisualization {
public:
    FluidVisualization(PixelMap& pixelMap);

    void Update(const Matrix& densityMap) override;

private:

//    std::unique_ptr<IDyeMap> _dyeMap;
    PixelMap& _pixelMap;

    uint8_t _singlePixelDyeAmount;

};