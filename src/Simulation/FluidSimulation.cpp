#include "FluidSimulation.h"
#include "../Utilities/Tracing.h"

/**
 * Constructs new fluid simulation abstraction
 * @param dimensions dimensions of the simulation (currently edge of x member is the size) - square
 * @param diffusion diffusion parameter
 * @param viscosity viscosity parameter
 * @param dt timestep
 */
FluidSimulation::FluidSimulation(Dimensions dimensions, double diffusion, double viscosity, double dt, int iterations)
    : _size((dimensions.x))
    , _diffusion(diffusion)
    , _viscosity(viscosity)
    , _dt(dt)
    , _iterations(iterations)
{
}

/**
 * Add density on given field. Normalization based on time step
 * (imagine dropping a drop of soy sauce into the water)
 * @param x x coordinate
 * @param y y coordinate
 * @param amount amount of density to be added
 * @param outDensity resulted density (normalized)
 */
void FluidSimulation::AddDensity(int x, int y, double amount, Matrix& outDensity) const
{
    outDensity(x, y) = (outDensity(x, y) + _dt * amount) / (1.0f + _dt);
}

/**
 * Add velocity on given place
 * (imagine on that pixel there is a fan blowing the particles)
 * @param x x coordinate
 * @param y y coordinate
 * @param amount amount of velocity to be added
 * @param velocity resulting velocity
 */
void FluidSimulation::AddVelocity(int x, int y, double amount, Matrix& outVelocity) const
{
    outVelocity(x, y) += amount;
}

/**
 * Evolve velocities due to:
 * - addition of forces (done even before Step method in AddVelocity)
 * - diffusion
 * - advection
 */
void FluidSimulation::VelocityStep(State& velocityX, State& velocityY) const
{
    Diffuse(BoundaryType::VERTICAL, velocityX.previous, velocityX.current, _viscosity);
    Diffuse(BoundaryType::HORIZONTAL, velocityY.previous, velocityY.current, _viscosity);

    Project(velocityX.previous, velocityY.previous, velocityX.current, velocityY.current);

    Advect(BoundaryType::VERTICAL, velocityX.current, velocityX.previous, velocityX.previous, velocityY.previous);

    Advect(BoundaryType::HORIZONTAL, velocityY.current, velocityY.previous, velocityX.previous, velocityY.previous);

    Project(velocityX.current, velocityY.current, velocityX.previous, velocityY.previous);
}

/**
 * Evolve density due to: addition of forces, addition of density, moving density along velocities, and diffusion
 */
void FluidSimulation::DensityStep(State& density, Matrix& velocityX, Matrix& velocityY) const
{
    Diffuse(BoundaryType::ALL, density.previous, density.current, _diffusion);
    Advect(BoundaryType::ALL, density.current, density.previous, velocityX, velocityY);
}

/**
 * Diffuse, spread medium across the grid no matter what velocity is.
 * To make this method stable, instead of calculating next surrounding values (and thus using linear interpolation),
 * a method of finding current values from the next ones is used. However this needs a solver - see a LinearSolve
 * method.
 * @param bound where a boundary condition should be st
 * @param medium medium that should be diffused
 * @param prevMedium previous medium
 * @param spreadSpeed viscosity/diffusion
 */
void FluidSimulation::Diffuse(
    FluidSimulation::BoundaryType bound, Matrix& medium, Matrix& prevMedium, double spreadSpeed) const
{
    double k = _dt * spreadSpeed * (_size - 2) * (_size - 2);

    LinearSolve(bound, medium, prevMedium, k, 1 + 4 * k);
}

/**
 * Use Gauss-Seidel method to solve linear equations
 * @param bound depending on what parameter is currently being solved
 * @param medium used medium
 * @param prevMedium previous medium
 * @param k amount of how much should the medium change
 * @param c denominator
 */
void FluidSimulation::LinearSolve(
    FluidSimulation::BoundaryType bound, Matrix& medium, Matrix& prevMedium, double k, double c) const
{
    for (int i = 0; i < _iterations; ++i) {
        for (int x = 1; x < _size - 1; ++x) {
            for (int y = 1; y < _size - 1; ++y) {
                medium(x, y) = (prevMedium(x, y)
                                   + k * (medium(x + 1, y) + medium(x - 1, y) + medium(x, y + 1) + medium(x, y - 1)))
                    / c;
            }
        }
        SetBoundaryConditions(bound, medium);
    }
}

/**
 * After all operations on fluid it is no longer divergence-free. This step uses Helmholtz decomposition
 * in order to get divergence-free fluid. This the fluid after this step will have a lot of "curl"
 * @param velocityX matrix of X velocities at each point
 * @param velocityY matrix of Y velocities at each point
   NOTE: Those two parameters are passed only to avoid allocating additional memory - they will be recalculated soon
         anyways
 * @param p curl free part
 * @param divergence divergence part
 */
void FluidSimulation::Project(Matrix& velocityX, Matrix& velocityY, Matrix& p, Matrix& divergence) const
{
    for (int x = 1; x < _size - 1; ++x) {
        for (int y = 1; y < _size - 1; ++y) {

            // calculate "divergence" velocities
            divergence(x, y) = -0.5f
                * (velocityX(x + 1, y) - velocityX(x - 1, y) + velocityY(x, y + 1) - velocityY(x, y - 1)) / _size;
            p(x, y) = 0;
        }
    }

    SetBoundaryConditions(BoundaryType::ALL, divergence);
    SetBoundaryConditions(BoundaryType::ALL, p);

    // make use of the solver to calculate p - a scalar value
    // gradient of p will become a curl free part, since curl of a gradient field is 0
    LinearSolve(BoundaryType::VERTICAL, p, divergence, 1, 4);

    for (int x = 1; x < _size - 1; x++) {
        for (int y = 1; y < _size - 1; y++) {

            velocityX(x, y) -= 0.5f * (p(x + 1, y) - p(x - 1, y)) * _size;

            velocityY(x, y) -= 0.5f * (p(x, y + 1) - p(x, y - 1)) * _size;
        }
    }

    SetBoundaryConditions(BoundaryType::VERTICAL, velocityX);
    SetBoundaryConditions(BoundaryType::HORIZONTAL, velocityY);
}

/**
 * Calculate linear interpolation between a and b for the parameter k (aka. std::lerp)
 * @param a first value
 * @param b second value
 * @param k interpolation point between a and b
 * @return linear interpolation between a and b
 */
double FluidSimulation::LinearInterpolation(double a, double b, double k) const
{
    return a + k * (b - a);
}

/**
 * Calculate how density will move due to velocities. A "back tracing" method is used -
 * taking previous density value at x,y then trace from where this value comes from. The result will be
 * somewhere in between grids, so a linear interpolation is used.
 * @param bound boundary
 * @param medium current density
 * @param prevMedium previous density
 * @param velocityX current X velocity matrix
 * @param velocityY current Y velocity matrix
 */
void FluidSimulation::Advect(FluidSimulation::BoundaryType bound, Matrix& medium, const Matrix& prevMedium,
    const Matrix& velocityX, const Matrix& velocityY) const
{
    // velocities for x and y coordinates
    double dtX = _dt * (_size - 2);
    double dtY = _dt * (_size - 2);

    // coordinates of point that was back traced
    double fx, fy;

    // integer parts of the back traced point
    int iX, iY;

    // fractional parts of the back traced point
    double jX, jY;

    for (int x = 1; x < _size - 1; ++x) {
        for (int y = 1; y < _size - 1; ++y) {

            // trace back where next density for i,j comes from
            fx = x - dtX * velocityX(x, y);
            fy = y - dtY * velocityY(x, y);

            // do not access points outside of the box during interpolation
            if (fx < 0.5) {
                fx = 0.5;
            }
            if (fx > _size + 0.5) {
                fx = _size - 2;
            }
            if (fy < 0.5) {
                fy = 0.5;
            }
            if (fy > _size + 0.5) {
                fy = _size - 2;
            }

            // integer part of coordinate
            iX = std::min(_size - 2, static_cast<int>(std::floor(fx)));
            iY = std::min(_size - 2, static_cast<int>(std::floor(fy)));

            // fractional part of coordinate
            jX = fx - iX;
            jY = fy - iY;

            // interpolate lower values (left and right bottom corner)
            auto z1 = LinearInterpolation(prevMedium(iX, iY), prevMedium(iX + 1, iY), jX);

            // interpolate upper values (left and right top corner)
            auto z2 = LinearInterpolation(prevMedium(iX, iY + 1), prevMedium(iX + 1, iY + 1), jX);

            medium(x, y) = LinearInterpolation(z1, z2, jY);
        }
    }

    SetBoundaryConditions(bound, medium);
}

/**
 * The fluid is assumed to be contained in a box so this function assures that:
 * - if a medium is a velocity: A No-Slip condition is met, and the appropriate velocity component is 0 relative
 *   to a boundary. That essentially means setting last cell near boundary minus penultimate cell
 *   (Prandtl no-slip boundary condition)
 *   (https://en.wikipedia.org/wiki/Boundary_layer https://en.wikipedia.org/wiki/No-slip_condition)
 * - if a medium is a density: As stated in: https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/GDC03.pdf
 *   we assume continuity - in case of density, last cells should be the same as penultimate cell
 * @param bound which type of boundary contidion will be handled
 * @param medium medium on which boundary contidion will be set
 */
void FluidSimulation::SetBoundaryConditions(FluidSimulation::BoundaryType bound, Matrix& medium) const
{

    for (int i = 0; i < _size - 1; ++i) {
        switch (bound) {
        case BoundaryType::VERTICAL:
            // last cells near vertical wall will be equal minus penultimate cell value
            // so the relative velocity to the boundary will be 0
            medium(0, i) = -medium(1, i);
            medium(_size - 1, i) = -medium(_size - 2, i);

            // last cells near horizontal wall will be equal to penultimate cell value
            medium(i, 0) = medium(i, 1);
            medium(i, _size - 1) = medium(i, _size - 2);
            break;
        case BoundaryType::HORIZONTAL:
            // same approach as above, but for movement along Y coordinate
            medium(i, 0) = -medium(i, 1);
            medium(i, _size - 1) = -medium(i, _size - 2);

            medium(0, i) = medium(1, i);
            medium(_size - 1, i) = medium(_size - 2, i);
            break;
        case BoundaryType::ALL:
            // in case of density, last cells should be the same as penultimate cell
            medium(i, 0) = medium(i, 1);
            medium(i, _size - 1) = medium(i, _size - 2);

            medium(0, i) = medium(1, i);
            medium(_size - 1, i) = medium(_size - 2, i);
            break;

        default:
            LOG_ERROR("Boundary type not handled in switch!\n");
        }
    }

    // boundaries at the corners, each corner is the average of it's neighbours
    medium(0, 0) = 0.5f * (medium(1, 0) + medium(0, 1));
    medium(0, _size - 1) = 0.5f * (medium(1, _size - 1) + medium(0, _size - 2));
    medium(_size - 1, 0) = 0.5f * (medium(_size - 2, 0) + medium(_size - 1, 1));
    medium(_size - 1, _size - 1) = 0.5f * (medium(_size - 2, _size - 1) + medium(_size - 1, _size - 2));
}
