#pragma once
#include "Dimensions.h"
#include "MouseInput.h"
#include "IFluid.h"
#include "Random.h"

void SimulationAutomatic(IFluid* fluid, const Dimensions& simulationSize, double deltaTime);

void SimulationManual(IFluid* fluid, MouseInput* mouseInput, double deltaTime);
