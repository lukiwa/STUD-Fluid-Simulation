#include "Fluid.h"
#include <memory>

/**
 * Constructs new fluid abstraction based on visualization and simulation component
 * @param simulation
 * @param visualization
 */
Fluid::Fluid(std::unique_ptr<IFluidSimulation> simulation, std::unique_ptr<IFluidVisualization> visualization, int size)
    : _simulation(std::move(simulation))
    , _visualization(std::move(visualization))
    , _size(size)
    , _velocityX(size)
    , _velocityY(size)
    , _density(size)
{
    for (int x = 0; x < _density.current.GetSize(); ++x) {
        for (int y = 0; y < _density.current.GetSize(); ++y) {
            _density.current(x, y) = 0;
        }
    }
}

/**
 * Do a time step on fluid.
 * Send density map to visualization to perform rendering
 */
void Fluid::Step(double deltaTime)
{
    _simulation->VelocityStep(_velocityX, _velocityY, deltaTime);
    _simulation->DensityStep(_density, _velocityX.current, _velocityY.current, deltaTime);
    _visualization->Update(_density.current);
}

/**
 * Add density on given field. Normalization based on time step
 * (imagine dropping a drop of soy sauce into the water)
 * @param x x coordinate
 * @param y y coordinate
 * @param amount amount of density to be added
 */
void Fluid::AddDensity(int x, int y, double amount)
{
    _simulation->AddDensity(x, y, amount, _density.current);
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
    _simulation->AddVelocity(x, y, amountX, _velocityX.current);
    _simulation->AddVelocity(x, y, amountY, _velocityY.current);
}
