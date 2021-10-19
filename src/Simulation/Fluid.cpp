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
 * @param radius of density
 */
void Fluid::AddDensity(int startingXCoordinate, int startingYCoordinate, double amount, int radius)
{
    FixBoundsAndRadius(startingXCoordinate, startingYCoordinate, radius);

    for (int x = 0; x < radius / 4; ++x) {
        for (int y = 0; y < radius / 4; ++y) {
            if (startingXCoordinate - x > 0 && startingYCoordinate - y > 0) {
                _simulation->AddDensity(startingXCoordinate - x, startingYCoordinate - y, amount, _density.current);
            }
            if (startingXCoordinate + x < _size && startingYCoordinate - y > 0) {
                _simulation->AddDensity(startingXCoordinate + x, startingYCoordinate - y, amount, _density.current);
            }
            if (startingXCoordinate + x < _size && startingYCoordinate + y < _size) {
                _simulation->AddDensity(startingXCoordinate + x, startingYCoordinate + y, amount, _density.current);
            }
            if (startingXCoordinate - x > 0 && startingYCoordinate + y < _size) {
                _simulation->AddDensity(startingXCoordinate - x, startingYCoordinate + y, amount, _density.current);
            }
        }
    }
}

/**
 * Add velocity on given place
 * (imagine on that pixel there is a fan blowing the particles)
 * @param x x coordinate
 * @param y y coordinate
 * @param amountX x velocity at this point
 * @param amountY y velocity at this point
 * @param radius radius of added velocity
 */
void Fluid::AddVelocity(int startingXCoordinate, int startingYCoordinate, double amountX, double amountY, int radius)
{
    FixBoundsAndRadius(startingXCoordinate, startingYCoordinate, radius);

    for (int x = 0; x < radius / 4; ++x) {
        for (int y = 0; y < radius / 4; ++y) {
            if (startingXCoordinate - x > 0 && startingYCoordinate - y > 0) {
                _simulation->AddVelocity(startingXCoordinate - x, startingYCoordinate - y, amountX, _velocityX.current);
                _simulation->AddVelocity(startingXCoordinate - x, startingYCoordinate - y, amountY, _velocityY.current);
            }
            if (startingXCoordinate + x < _size && startingYCoordinate - y > 0) {
                _simulation->AddVelocity(startingXCoordinate + x, startingYCoordinate - y, amountX, _velocityX.current);
                _simulation->AddVelocity(startingXCoordinate + x, startingYCoordinate - y, amountY, _velocityY.current);
            }
            if (startingXCoordinate + x < _size && startingYCoordinate + y < _size) {
                _simulation->AddVelocity(startingXCoordinate + x, startingYCoordinate + y, amountX, _velocityX.current);
                _simulation->AddVelocity(startingXCoordinate + x, startingYCoordinate + y, amountY, _velocityY.current);
            }
            if (startingXCoordinate - x > 0 && startingYCoordinate + y < _size) {
                _simulation->AddVelocity(startingXCoordinate - x, startingYCoordinate + y, amountX, _velocityX.current);
                _simulation->AddVelocity(startingXCoordinate - x, startingYCoordinate + y, amountY, _velocityY.current);
            }
        }
    }
}

/**
 * Set x and y to be in range of fluid size. Sets radius to be no smaller than 4
 * @param outX x coordinate
 * @param outY y coordinate
 * @param outRadius radius
 */
void Fluid::FixBoundsAndRadius(int& outX, int& outY, int& outRadius) const
{
    outX = std::min(outX, _size - 1);
    outY = std::min(outY, _size - 1);
    outX = std::max(outX, 0);
    outY = std::max(outY, 0);
    if (outRadius < 4) {
        outRadius = 4;
    }
}
