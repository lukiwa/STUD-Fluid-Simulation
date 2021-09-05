#include "Fluid.h"
#include <memory>

Fluid::Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization)
    : _simulation(std::move(simulation))
    , _visualization(std::move(visualization))
{
}
