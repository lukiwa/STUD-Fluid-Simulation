#pragma once
#include "Fluid.h"
#include "Dimensions.h"

class FluidBuilder {

public:
    FluidBuilder();

    ~FluidBuilder() = default;

    FluidBuilder& Size(Dimensions dimensions);

    FluidBuilder& DyeMatrix(PixelMap& pixelMap);

    FluidBuilder& TimeStep(double dt);

    FluidBuilder& Diffusion(double diffusion);

    FluidBuilder& Viscosity(double viscosity);

    std::unique_ptr<Fluid> Build();

private:
    Dimensions _dimensions;
    std::unique_ptr<IDyeMap> _dyeMap;
    double _dt;
    double _diffusion;
    double _viscosity;
};