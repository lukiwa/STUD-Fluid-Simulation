#include "Fluid.h"
#include <memory>

/**
 * Constructs new fluid abstraction based on visualization and simulation component
 * @param simulation
 * @param visualization
 */
Fluid::Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization)
    : _simulation(std::move(simulation))
    , _visualization(std::move(visualization))
{
}

/**
 * Do a time step on fluid.
 * Send density map to visualization to perform rendering
 */
void Fluid::Step()
{
    _simulation->Step();
    _visualization->Step(_simulation->CurrentDensityMap());
}

/**
 * Add density on given field
 * (imagine dropping a drop of soy sauce into the water)
 * @param x x coordinate
 * @param y y coordinate
 * @param amount amount of density to be added
 */
void Fluid::AddDensity(int x, int y, double amount)
{
    _simulation->AddDensity(x, y, amount);
}

/**
 * Add velocity on given place
 * (imagine on that pixel there is a fan blowing the particles)
 * @param x x coordinate
 * @param y y coordinate
 * @param amountX x velocity at this point
 * @param amountY y velocity at this point
 */
void Fluid::AddVelocity(int x, int y, double amountX, double amountY)
{
    _simulation->AddVelocity(x, y, amountX, amountY);
}