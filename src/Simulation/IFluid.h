#pragma once
#include "../Utilities/Array2D.h"

class IFluidSimulation {
public:
    virtual ~IFluidSimulation() = default;
    virtual void Step() = 0;
    virtual const Array2D<double>& CurrentDensityMap() const = 0;
    virtual void AddVelocity(int x, int y, double amountX, double amountY) = 0;
    virtual void AddDensity(int x, int y, double amount) = 0;
};

class IFluidVisualization {
public:
    virtual ~IFluidVisualization() = default;
    virtual void Step(const Array2D<double>& densityMap) = 0;
};