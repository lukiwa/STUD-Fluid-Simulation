#pragma once
#include "Dimensions.h"
#include "Fluid.h"
#include "FluidSimulation.h"

class FluidVisualizationBuilder {
public:
    FluidVisualizationBuilder();
    ~FluidVisualizationBuilder() = default;
    FluidVisualizationBuilder& PixelMatrix(IPixelMap* pixelMap);
    std::unique_ptr<IFluidVisualization> Build();

private:
    IPixelMap* _pixelMap;
};

class FluidSimulationBuilder {
public:
    FluidSimulationBuilder();
    ~FluidSimulationBuilder() = default;

    FluidSimulationBuilder& Size(Dimensions dimensions);
    FluidSimulationBuilder& Diffusion(double diffusion);
    FluidSimulationBuilder& Viscosity(double viscosity);
    FluidSimulationBuilder& TimeStep(double dt);
    FluidSimulationBuilder& Iterations(int iterations);
    std::unique_ptr<IFluidSimulation> Build();

private:
    Dimensions _dimensions;
    double _dt;
    double _diffusion;
    double _viscosity;
    int _iterations;
};

class FluidBuilder {
public:
    FluidBuilder();
    ~FluidBuilder() = default;
    FluidBuilder& Size(Dimensions dimensions);
    FluidSimulationBuilder& Simulation();
    FluidVisualizationBuilder& Visualization();
    std::unique_ptr<IFluid> Build();

private:
    Dimensions _dimensions;
    FluidSimulationBuilder _simulationBuilder;
    FluidVisualizationBuilder _visualizationBuilder;
};
