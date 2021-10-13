#pragma once
#include "IFluid.h"
#include "../Utilities/Array2D.h"
#include "DyeMap.h"


class FluidVisualization : public IFluidVisualization {
public:
    FluidVisualization(std::unique_ptr<IDyeMap> dyeMap);

    void Update(const Array2D<double>& densityMap) override;

private:
    std::unique_ptr<IDyeMap> _dyeMap;
};