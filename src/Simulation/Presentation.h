#pragma once
#include "../Utilities/Dimensions.h"
#include "../Utilities/MouseInput.h"
#include "IFluid.h"
#include "../Utilities/Random.h"

void SimulationAutomatic(IFluid* fluid, const Dimensions& simulationSize, double deltaTime);

void SimulationManual(IFluid* fluid, MouseInput* mouseInput, double deltaTime);
