#pragma once
#include "../Utilities/Matrix.h"

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
        Matrix previous;
        Matrix current;
    };

    virtual void AddVelocity(int x, int y, double amount, Matrix& velocity) const = 0;
    virtual void AddDensity(int x, int y, double amount, Matrix& density) const = 0;
    virtual void Diffuse(BoundaryType bound, Matrix& medium, Matrix& prevMedium, double spreadSpeed) const = 0;
    virtual void LinearSolve(BoundaryType bound, Matrix& medium, Matrix& prevMedium, double k, double c) const = 0;
    virtual void Project(Matrix& velocityX, Matrix& velocityY, Matrix& p, Matrix& divergence) const = 0;
    virtual void Advect(BoundaryType bound, Matrix& medium, const Matrix& prevMedium, const Matrix& velocityX,
        const Matrix& velocityY) const = 0;
    virtual void SetBoundaryConditions(BoundaryType bound, Matrix& medium) const = 0;

    virtual void VelocityStep(State& velocityX, State& velocityY) const = 0;
    virtual void DensityStep(State& density, Matrix& velocityX, Matrix& velocityY) const = 0;
};

class IFluidVisualization {
public:
    virtual ~IFluidVisualization() = default;
    virtual void Update(const Matrix& densityMap) = 0;
};