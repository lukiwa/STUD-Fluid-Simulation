#pragma once
#include "../Utilities/Array2D.h"
#include "FluidSimulation.h"
#include "FluidVisualisation.h"

class Fluid {
public:
    void AddVelocity(int x, int y, double amountX, double amountY);
    void AddDensity(int x, int y, double amount);
    void Step();

private:
    friend class FluidBuilder;
    Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization);
    std::unique_ptr<IFluidSimulation> _simulation;
    std::unique_ptr<IFluidVisualization> _visualization;
};
