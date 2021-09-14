#include "FluidSimulation.h"

/**
 * Constructs new fluid simulation abstraction
 * @param dimensions dimensions of the simulation (currently edge of x member is the size) - square
 * @param diffusion diffusion parameter
 * @param viscosity viscosity parameter
 * @param dt timestep
 */
FluidSimulation::FluidSimulation(Dimensions dimensions, double diffusion, double viscosity, double dt)

    //: _size((dimensions.x + 2) * (dimensions.y + 2))
    : _size((dimensions.x))
    , _diffusion(diffusion)
    , _viscosity(viscosity)
    , _dt(dt)
    , _velocityX(_size)
    , _velocityY(_size)
    , _density(_size)
{
    for (int x = 0; x < _density.current.GetSize(); ++x) {
        for (int y = 0; y < _density.current.GetSize(); ++y) {
            _density.current(x, y) = 0;
        }
    }
}

/**
 * Get current density map
 * @return current density map
 */
const Array2D<double>& FluidSimulation::CurrentDensityMap() const
{
    {
        return _density.current;
    }
}

/**
 * Add velocity on given place
 * (imagine on that pixel there is a fan blowing the particles)
 * @param x x coordinate
 * @param y y coordinate
 * @param amountX x velocity at this point
 * @param amountY y velocity at this point
 */
void FluidSimulation::AddVelocity(int x, int y, double amountX, double amountY)
{
    _velocityX.current(x, y) += amountX;
    _velocityY.current(x, y) += amountY;
}

/**
 * Add density on given field
 * (imagine dropping a drop of soy sauce into the water)
 * @param x x coordinate
 * @param y y coordinate
 * @param amount amount of density to be added
 */
void FluidSimulation::AddDensity(int x, int y, double amount)
{
    _density.current(x, y) += amount;
}

/**
 * Performs calculation to get new fluid state
 */
void FluidSimulation::Step()
{
    // TODO
}
