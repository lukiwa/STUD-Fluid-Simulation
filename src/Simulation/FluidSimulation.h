#pragma once
#include "../Utilities/Array2D.h"
#include "../Utilities/Dimensions.h"
#include "IFluid.h"
#include <chrono>
#include <cmath>

class FluidSimulation : public IFluidSimulation {
public:
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

    enum class BoundaryType { VERTICAL, HORIZONTAL, ALL };

    const Array2D<double>& CurrentDensityMap() const override;
    void AddVelocity(int x, int y, double amountX, double amountY) override;
    void AddDensity(int x, int y, double amount) override;
    void Step() override;

private:
    void Diffuse(BoundaryType bound, Array2D<double>& medium, Array2D<double>& prevMedium, double spreadSpeed);
    void LinearSolve(BoundaryType bound, Array2D<double>& medium, Array2D<double>& prevMedium, double k, double c);
    void Project(
        Array2D<double>& velocityX, Array2D<double>& velocityY, Array2D<double>& p, Array2D<double>& divergence);
    void Advect(BoundaryType bound, Array2D<double>& medium, const Array2D<double>& prevMedium,
        const Array2D<double>& velocityX, const Array2D<double>& velocityY);
    void SetBoundaryConditions(BoundaryType bound, Array2D<double>& medium);

private:
    void VelocityStep();
    void DensityStep();
    double LinearInterpolation(double a, double b, double k) const;

    friend class FluidBuilder;
    FluidSimulation(Dimensions dimensions, double diffusion, double viscosity, double dt);

    int _size;
    double _diffusion;
    double _viscosity;
    double _dt;
    int _iterations;
    State _velocityX;
    State _velocityY;
    State _density;
};
