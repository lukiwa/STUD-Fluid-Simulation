#include "FluidBuilder.h"
#include <memory>

/*
FluidBuilder& FluidBuilder::Size(Dimensions dimensions)
{
    _dimensions = dimensions;
    return *this;
}
FluidBuilder& FluidBuilder::DyeMatrix(PixelMap& pixelMap)
{
    _dyeMap = std::make_unique<DyeMap>(pixelMap);
    return *this;
}
FluidBuilder& FluidBuilder::TimeStep(double dt)
{
    _dt = dt;
    return *this;
}
FluidBuilder& FluidBuilder::Diffusion(double diffusion)
{
    _diffusion = diffusion;
    return *this;
}
FluidBuilder& FluidBuilder::Viscosity(double viscosity)
{
    _viscosity = viscosity;
    return *this;
}
std::unique_ptr<Fluid> FluidBuilder::Build()
{
    std::unique_ptr<IFluidVisualization> visualization(new FluidVisualization(std::move(_dyeMap)));
    std::unique_ptr<IFluidSimulation> simulation(new FluidSimulation(_dimensions, _diffusion, _viscosity, _dt));
    std::unique_ptr<Fluid> fluid(new Fluid(std::move(simulation), std::move(visualization), _dimensions.x, _dt));
    return fluid;
}
FluidBuilder::FluidBuilder()
    : _dimensions()
    , _dyeMap(nullptr)
    , _dt(0)
    , _diffusion(0)
    , _viscosity(0)
{
}
*/

// VISUALIZATION BUILDER
/**
 * Creates new fluid visualization builder
 */
FluidBuilder::FluidVisualizationBuilder::FluidVisualizationBuilder()
    : _dyeMap(nullptr)
{
}
/**
 * Selects pixel map to use for visualization
 * @param pixelMap pixel map
 * @return builder reference
 */
FluidBuilder::FluidVisualizationBuilder& FluidBuilder::FluidVisualizationBuilder::PixelMatrix(PixelMap& pixelMap)
{
    _dyeMap = std::make_unique<DyeMap>(pixelMap);
    return *this;
}

/**
 * Builds new fluid visualization component
 * @return builder reference
 */
std::unique_ptr<IFluidVisualization> FluidBuilder::FluidVisualizationBuilder::Build()
{
    std::unique_ptr<IFluidVisualization> visualization(new FluidVisualization(std::move(_dyeMap)));
    return visualization;
}

// SIMULATION BUILDER
/**
 * Creates new simulation builder
 */
FluidBuilder::FluidSimulationBuilder::FluidSimulationBuilder()
    : _dimensions()
    , _dt(0)
    , _diffusion(0)
    , _viscosity(0)
    , _iterations(4)
{
}

/**
 * Set simulation size
 * @param dimensions dimension of the box
 * @return builder reference
 */
FluidBuilder::FluidSimulationBuilder& FluidBuilder::FluidSimulationBuilder::Size(Dimensions dimensions)
{
    _dimensions = dimensions;
    return *this;
}

/**
 * Set diffusion
 * @param diffusion diffusion parameter
 * @return builder reference
 */
FluidBuilder::FluidSimulationBuilder& FluidBuilder::FluidSimulationBuilder::Diffusion(double diffusion)
{
    _diffusion = diffusion;
    return *this;
}
/**
 * Set viscosity
 * @param viscosity viscosity parameter
 * @return builder reference
 */
FluidBuilder::FluidSimulationBuilder& FluidBuilder::FluidSimulationBuilder::Viscosity(double viscosity)
{
    _viscosity = viscosity;
    return *this;
}
/**
 * Set timestep
 * @param dt timestep parameter
 * @return builder reference
 */
FluidBuilder::FluidSimulationBuilder& FluidBuilder::FluidSimulationBuilder::TimeStep(double dt)
{
    _dt = dt;
    return *this;
}

/**
 * Set iterations for Gauss-Seidel method
 * @param iterations iterations parameters
 * @return builder reference
 */
FluidBuilder::FluidSimulationBuilder& FluidBuilder::FluidSimulationBuilder::Iterations(int iterations)
{
    _iterations = iterations;
    return *this;
}

/**
 * Constructs fluid simulation component
 * @return fluid ismulation component
 */
std::unique_ptr<IFluidSimulation> FluidBuilder::FluidSimulationBuilder::Build()
{
    std::unique_ptr<IFluidSimulation> simulation(
        new FluidSimulation(_dimensions, _diffusion, _viscosity, _dt, _iterations));

    return simulation;
}

// FLUID BUILDER
/**
 * Constructs new fluid builder
 */
FluidBuilder::FluidBuilder()
    : _dimensions()
    , _simulationBuilder()
    , _visualizationBuilder()
{
}

/**
 * Dimension of the simulation
 * @param dimensions dimension parameter
 * @return builder reference
 */
FluidBuilder& FluidBuilder::Size(Dimensions dimensions)
{
    _dimensions = dimensions;
    _simulationBuilder.Size(dimensions);
    return *this;
}
/**
 * Get simulation builder
 * @return simulation builder reference
 */
FluidBuilder::FluidSimulationBuilder& FluidBuilder::Simulation()
{
    return _simulationBuilder;
}

/**
 * Get visualization builder
 * @return visualization builder reference
 */
FluidBuilder::FluidVisualizationBuilder& FluidBuilder::Visualization()
{
    return _visualizationBuilder;
}

/**
 * Constructs new fluid abstraction
 * @return fluid abstraction
 */
std::unique_ptr<IFluid> FluidBuilder::Build()
{
    auto simulation = _simulationBuilder.Build();
    auto visualization = _visualizationBuilder.Build();
    std::unique_ptr<IFluid> fluid(new Fluid(std::move(simulation), std::move(visualization), _dimensions.x));
    return fluid;
}
