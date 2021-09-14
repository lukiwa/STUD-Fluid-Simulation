#pragma once
#include "IFluid.h"
#include "../Utilities/Array2D.h"
#include "../Utilities/Dimensions.h"

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

    const Array2D<double>& CurrentDensityMap() const override;

    void AddVelocity(int x, int y, double amountX, double amountY) override;
    void AddDensity(int x, int y, double amount) override;
    void Step() override;

private:
    friend class FluidBuilder;
    FluidSimulation(Dimensions dimensions, double diffusion, double viscosity, double dt);

    int _size;
    double _diffusion;
    double _viscosity;
    double _dt;
    State _velocityX;
    State _velocityY;
    State _density;
};

