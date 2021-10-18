#include "Presentation.h"

/**
 * Simulation is presented automatically. There is a point in the middle of the screen where a density, and random
 * velocity is added, so the density spreads randomly
 * @param fluid fluid
 * @param simulationSize size of the simulation to calculate middle of the screen
 * @param deltaTime delta time
 */
void SimulationAutomatic(IFluid* fluid, const Dimensions& simulationSize, double deltaTime)
{
    fluid->AddDensity(simulationSize.x / 2, simulationSize.y / 2, Random::Int(150, 200), 10);
    fluid->AddVelocity(simulationSize.x / 2, simulationSize.y / 2, Random::Double(-50, 50), Random::Double(-50, 50), 4);
    fluid->Step(deltaTime);
}

/**
 * Simulation can be controlled by user using mouse.
 * LMB - add density
 * RMB - add velocity, the stronger the mouse drag during button press the stronger the velocity at given direction
 * Mouse Scroll - control size of the density and velocity to be added
 * @param fluid  fluid
 * @param mouseInput class to catch input from mouse
 * @param deltaTime delta time
 */
void SimulationManual(IFluid* fluid, MouseInput* mouseInput, double deltaTime)
{
    static int prevX = 0;
    static int prevY = 0;
    static double deltaX = 0;
    static double deltaY = 0;
    static int radius = 10;
    mouseInput->WheelCallback([&](bool moveUp) {
        if (moveUp) {
            radius = std::min(radius + 1, 20);
        } else {
            radius = std::max(radius - 1, 4);
        }
    });

    if (mouseInput->CheckMouseButton(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS)) {
        auto [x, y] = mouseInput->MappedMousePosition();
        fluid->AddDensity(x, y, 500, radius);
    }

    if (mouseInput->CheckMouseButton(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS)) {
        auto [x, y] = mouseInput->MappedMousePosition();

        deltaX = std::min(x - prevX, 50);
        deltaY = std::min(y - prevY, 50);

        fluid->AddVelocity(x, y, deltaX, deltaY, radius);

        auto result = mouseInput->MappedMousePosition();
        prevX = result.first;
        prevY = result.second;
    }

    fluid->Step(deltaTime);
}
