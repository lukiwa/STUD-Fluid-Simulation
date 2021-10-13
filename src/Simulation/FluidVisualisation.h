#pragma once
#include "../Utilities/Matrix.h"
#include "DyeMap.h"
#include "IFluid.h"

class FluidVisualization : public IFluidVisualization {
public:
    FluidVisualization(std::unique_ptr<IDyeMap> dyeMap);

    void Update(const Matrix& densityMap) override;

private:
    std::unique_ptr<IDyeMap> _dyeMap;
};