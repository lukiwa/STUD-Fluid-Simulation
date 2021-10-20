#pragma once
#include "Matrix.h"
#include "FluidSimulation.h"
#include "FluidVisualisation.h"

class Fluid : IFluid {
public:
    void AddVelocity(int x, int y, double amountX, double amountY, int radius) override;
    void AddDensity(int x, int y, double amount, int radius) override;
    void Step(double deltaTime) override;

private:
    void FixBoundsAndRadius(int& outX, int& outY, int& outRadius) const;
    friend class FluidBuilder;
    Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization, int size);
    std::unique_ptr<IFluidSimulation> _simulation;
    std::unique_ptr<IFluidVisualization> _visualization;

    int _size;
    IFluidSimulation::State _velocityX;
    IFluidSimulation::State _velocityY;
    IFluidSimulation::State _density;
};
