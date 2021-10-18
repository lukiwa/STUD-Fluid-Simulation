#include "Utilities/Tracing.h"
#include <fstream>
#include <iostream>
#include <memory>

#include "ImGuiHandler.h"
#include "Matrix.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/PixelMapBuilder.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/VertexAttributes.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/Window.h"
#include "Utilities/MouseInput.h"
#include "Utilities/Random.h"

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SimulationAutomatic(IFluid* fluid, const Dimensions& simulationSize, double deltaTime)
{
    fluid->AddDensity(simulationSize.x / 2, simulationSize.y / 2, Random::Int(150, 200), 10);
    fluid->AddVelocity(simulationSize.x / 2, simulationSize.y / 2, Random::Double(-50, 50), Random::Double(-50, 50), 4);
    fluid->Step(deltaTime);
}

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

int main(int, char**)
{
    ALLOW_DISPLAY;
    Random::Seed();
    GLFW::Window window(512, 512, "Fluid simulation");

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize OpenGL loader!");
    }

    VertexArray vao;

    // clang-format off
    GLfloat positions[] = {
        1.0, 1.0f,     1.0f, 1.0f, // top right
        1.0f, -1.0f,    1.0f, 0.0f, // bottom right
        -1.0f, -1.0f,   0.0f, 0.0f, // bottom left
        -1.0f, 1.0f,    0.0f, 1.0f // top left
    };
    // clang-format on
    VertexBuffer vertexBuffer(positions, sizeof(positions));
    VertexAttributes attributes;
    attributes.AddAttribute(2, GL_FLOAT); // positions
    attributes.AddAttribute(2, GL_FLOAT); // texture coordinates
    attributes.Bind();

    GLuint indices[] = {
        0, 1, 3, // first  triangle
        1, 2, 3 // second triangle
    };
    IndexBuffer indexBuffer(indices, 6);

    ShaderProgramBuilder builder;
    int validationResult = 0;
    auto program = builder.ShaderFilename(GL_VERTEX_SHADER, "vertex_shader.glsl")
                       .ShaderFilename(GL_FRAGMENT_SHADER, "fragment_shader.glsl")
                       .CreateProgram()
                       .CompileShader(GL_VERTEX_SHADER)
                       .CompileShader(GL_FRAGMENT_SHADER)
                       .LinkShaders()
                       .Validate(validationResult)
                       .Build();

    // PixelMap
    std::unique_ptr<PixelMap> pixelMap;
    std::unique_ptr<PixelMapComponentsFactory> componentsFactory(new PixelMapComponentsFactory());
    PixelMapBuilder pixelMapBuilder;
    pixelMapBuilder.Factory(componentsFactory.get()).PixelFormat(GL_RGBA);

    // Fluid
    std::unique_ptr<IFluid> fluid;
    FluidBuilder fluidBuilder;

    // ImGui
    ImGui::Handler imguiHandler(window.GetHandle());
    std::unique_ptr<ImGui::IImGuiWindow> beginWindow = std::make_unique<ImGui::BeginWindow>(
        window.GetWidth(), window.GetHeight(), fluidBuilder, pixelMapBuilder, imguiHandler);
    std::unique_ptr<ImGui::IImGuiWindow> fpsWindow = std::make_unique<ImGui::FpsWindow>();

    Renderer renderer(vao, indexBuffer, program, pixelMap.get(), imguiHandler);
    double currentFrame = 0, deltaTime = 0, lastFrame = 0;

    bool automaticSimulation = false;

    std::unique_ptr<MouseInput> mouseInput;

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

                renderer.SetPixelMap(pixelMap.get());

                automaticSimulation = fluidBuilder.Visualization().IsAutomaticSimulation();
                if (!automaticSimulation) {
                    mouseInput = std::make_unique<MouseInput>(pixelMap->GetWidth(), pixelMap->GetHeight(),
                        window.GetWidth(), window.GetHeight(), window.GetHandle());
                }
            })) {
            beginWindow->Draw();
            renderer.Clear();

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

        renderer.Draw();
        window.SwapBuffers();
    }
    return 0;
}
