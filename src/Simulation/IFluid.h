#pragma once
#include "../Utilities/Array2D.h"

class IFluid {
public:
    virtual ~IFluid() = default;
    virtual void AddVelocity(int x, int y, double amountX, double amountY) = 0;
    virtual void AddDensity(int x, int y, double amount) = 0;
    virtual void Step() = 0;
};

class IFluidSimulation {
public:
    virtual ~IFluidSimulation() = default;
    enum class BoundaryType { VERTICAL, HORIZONTAL, ALL };
    struct State {
        /**
         * Contains information about previous and current state of simulation
         * @param size size of the simulation
         */
        explicit State(int size)
            : previous(size)
            , current(size)
        {
        }
        Array2D<double> previous;
        Array2D<double> current;
    };

    virtual void Diffuse(
        BoundaryType bound, Array2D<double>& medium, Array2D<double>& prevMedium, double spreadSpeed) const = 0;
    virtual void LinearSolve(
        BoundaryType bound, Array2D<double>& medium, Array2D<double>& prevMedium, double k, double c) const = 0;
    virtual void Project(Array2D<double>& velocityX, Array2D<double>& velocityY, Array2D<double>& p,
        Array2D<double>& divergence) const = 0;
    virtual void Advect(BoundaryType bound, Array2D<double>& medium, const Array2D<double>& prevMedium,
        const Array2D<double>& velocityX, const Array2D<double>& velocityY) const = 0;
    virtual void SetBoundaryConditions(BoundaryType bound, Array2D<double>& medium) const = 0;

    virtual void VelocityStep(State& velocityX, State& velocityY) const = 0;
    virtual void DensityStep(State& density, Array2D<double>& velocityX, Array2D<double>& velocityY) const = 0;
};

class IFluidVisualization {
public:
    virtual ~IFluidVisualization() = default;
    virtual void Update(const Array2D<double>& densityMap) = 0;
};