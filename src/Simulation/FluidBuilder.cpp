#include "FluidBuilder.h"

#include <memory>
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
    std::unique_ptr<IFluidSimulation> simulation(new FluidSimulation());
    std::unique_ptr<Fluid> fluid(new Fluid(std::move(simulation), std::move(visualization)));
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
