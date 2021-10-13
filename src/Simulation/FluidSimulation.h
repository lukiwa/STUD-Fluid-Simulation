#pragma once
#include "../Utilities/Dimensions.h"
#include "../Utilities/Matrix.h"
#include "IFluid.h"
#include <chrono>
#include <cmath>

class FluidSimulation : public IFluidSimulation {
public:
    void AddVelocity(int x, int y, double amount, Matrix& velocity) const override;
    void AddDensity(int x, int y, double amount, Matrix& density) const override;
    void VelocityStep(State& velocityX, State& velocityY) const override;
    void DensityStep(State& density, Matrix& velocityX, Matrix& velocityY) const override;
    void Diffuse(BoundaryType bound, Matrix& medium, Matrix& prevMedium, double spreadSpeed) const override;
    void LinearSolve(BoundaryType bound, Matrix& medium, Matrix& prevMedium, double k, double c) const override;
    void Project(Matrix& velocityX, Matrix& velocityY, Matrix& p, Matrix& divergence) const override;
    void Advect(BoundaryType bound, Matrix& medium, const Matrix& prevMedium, const Matrix& velocityX,
        const Matrix& velocityY) const override;
    void SetBoundaryConditions(BoundaryType bound, Matrix& medium) const override;

private:
    double LinearInterpolation(double a, double b, double k) const;

    friend class FluidBuilder;
    FluidSimulation(Dimensions dimensions, double diffusion, double viscosity, double dt, int iterations);

    int _size;
    double _diffusion;
    double _viscosity;
    double _dt;
    int _iterations;
};
