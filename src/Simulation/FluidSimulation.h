#pragma once
#include "../Utilities/Array2D.h"
#include "../Utilities/Dimensions.h"
#include "IFluid.h"
#include <chrono>
#include <cmath>

class FluidSimulation : public IFluidSimulation {
public:
    void VelocityStep(State& velocityX, State& velocityY) const override;

    void DensityStep(State& density, Array2D<double>& velocityX, Array2D<double>& velocityY) const override;

    void Diffuse(
        BoundaryType bound, Array2D<double>& medium, Array2D<double>& prevMedium, double spreadSpeed) const override;
    void LinearSolve(
        BoundaryType bound, Array2D<double>& medium, Array2D<double>& prevMedium, double k, double c) const override;
    void Project(Array2D<double>& velocityX, Array2D<double>& velocityY, Array2D<double>& p,
        Array2D<double>& divergence) const override;
    void Advect(BoundaryType bound, Array2D<double>& medium, const Array2D<double>& prevMedium,
        const Array2D<double>& velocityX, const Array2D<double>& velocityY) const override;
    void SetBoundaryConditions(BoundaryType bound, Array2D<double>& medium) const override;

private:
    double LinearInterpolation(double a, double b, double k) const;

    friend class FluidBuilder;
    FluidSimulation(Dimensions dimensions, double diffusion, double viscosity, double dt);

    int _size;
    double _diffusion;
    double _viscosity;
    double _dt;
    int _iterations;
};
