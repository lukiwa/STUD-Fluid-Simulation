#include "Utilities/Tracing.h"
#include <Engine.h>
#include <iostream>
#include <memory>

#include "ImGuiHandler.h"
#include "OpenGL/Window.h"
#include "Utilities/MouseInput.h"
#include "Utilities/Random.h"
#include "Simulation/Presentation.h"

int main(int, char**)
{
    Random::Seed();

    // window
    ALLOW_DISPLAY;
    GLFW::Window window(768, 768, "Fluid simulation");

    // Glew
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize OpenGL loader!");
    }

    // Drawing engine
    Engine drawingEngine;
    auto renderer = drawingEngine.Init();

    // PixelMap
    PixelMapBuilder pixelMapBuilder;
    std::unique_ptr<PixelMap> pixelMap;
    std::unique_ptr<PixelMapComponentsFactory> componentsFactory(new PixelMapComponentsFactory());
    pixelMapBuilder.Factory(componentsFactory.get()).PixelFormat(GL_RGBA);

    // Fluid
    FluidBuilder fluidBuilder;
    std::unique_ptr<IFluid> fluid;
    bool automaticSimulation = false;

    // ImGui
    ImGui::Handler imguiHandler(window.GetHandle());
    std::unique_ptr<ImGui::IImGuiWindow> beginWindow = std::make_unique<ImGui::BeginWindow>(
        window.GetWidth(), window.GetHeight(), fluidBuilder, pixelMapBuilder, imguiHandler);
    std::unique_ptr<ImGui::IImGuiWindow> fpsWindow = std::make_unique<ImGui::FpsWindow>();
    renderer->SetImGuiHandler(&imguiHandler);

    // Mouse input
    std::unique_ptr<MouseInput> mouseInput;

    // delta time
    double currentFrame = 0, deltaTime = 0, lastFrame = 0;

    while (!window.ShouldClose()) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.ProcessInput();
        imguiHandler.NewFrame();

        if (!beginWindow->ShouldClose([&]() {
                pixelMap = pixelMapBuilder.Build();
                pixelMap->Clear();

                fluidBuilder.Visualization().PixelMatrix(pixelMap.get());
                fluid = fluidBuilder.Build();

                renderer->SetPixelMap(pixelMap.get());

                automaticSimulation = fluidBuilder.Visualization().IsAutomaticSimulation();
                if (!automaticSimulation) {
                    mouseInput = std::make_unique<MouseInput>(pixelMap->GetWidth(), pixelMap->GetHeight(),
                        window.GetWidth(), window.GetHeight(), window.GetHandle());
                }
            })) {
            beginWindow->Draw();
            renderer->Clear();

        } else {
#ifdef DEBUG
            fpsWindow->Draw();
#endif

            if (automaticSimulation) {
                SimulationAutomatic(fluid.get(), { pixelMap->GetWidth(), pixelMap->GetHeight(), 0 }, deltaTime);
            } else {
                SimulationManual(fluid.get(), mouseInput.get(), deltaTime);
            }
        }

        renderer->Draw();
        window.SwapBuffers();
    }
    return 0;
}
