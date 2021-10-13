#pragma once
#include "../Utilities/Matrix.h"
#include "FluidSimulation.h"
#include "FluidVisualisation.h"

class Fluid : IFluid {
public:
    void AddVelocity(int x, int y, double amountX, double amountY) override;
    void AddDensity(int x, int y, double amount) override;
    void Step() override;


private:



    friend class FluidBuilder;
    Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization, int size, double dt);
    std::unique_ptr<IFluidSimulation> _simulation;
    std::unique_ptr<IFluidVisualization> _visualization;

    int _size;
    double _dt;
    IFluidSimulation::State _velocityX;
    IFluidSimulation::State _velocityY;
    IFluidSimulation::State _density;
};
