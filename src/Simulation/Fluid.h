#pragma once
#include "DyeMap.h"
#include "IFluid.h"

class FluidVisualization : public IFluidVisualization {
public:
    FluidVisualization(std::unique_ptr<IDyeMap> dyeMap)
        : _dyeMap(std::move(dyeMap))
    {
    }


    void Step() override
    {
    }

private:
    std::unique_ptr<IDyeMap> _dyeMap;
};

class FluidSimulation : public IFluidSimulation {
public:
};

class Fluid {
public:
    Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization);

private:
    std::unique_ptr<IFluidSimulation> _simulation;
    std::unique_ptr<IFluidVisualization> _visualization;
};
