#include "Utilities/Tracing.h"
#include <fstream>
#include <iostream>
#include <memory>

#include "ImGuiHandler.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/VertexAttributes.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/Window.h"
#include "Utilities/Random.h"

#include "Matrix.h"

int main(int, char**)
{
    ALLOW_DISPLAY;
    Random::Seed();
    GLFW::Window window(256, 256, "Fluid simulation");

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

    std::unique_ptr<PixelMapComponentsFactory> componentsFactory(new PixelMapComponentsFactory());
    PixelMap pixelMap({ window.GetWidth(), window.GetHeight(), 0 }, GL_RGBA, componentsFactory.get());
    pixelMap.Clear();

    // Fluid
    FluidBuilder fluidBuilder;
    std::unique_ptr<Fluid> fluid;
    fluidBuilder.Size({ pixelMap.GetWidth(), pixelMap.GetHeight(), 0 }).DyeMatrix(pixelMap);

    // ImGui
    ImGui::Handler imguiHandler(window.GetHandle());
    std::unique_ptr<ImGui::IImGuiWindow> beginWindow
        = std::make_unique<ImGui::BeginWindow>(window.GetWidth(), fluidBuilder, imguiHandler);
    std::unique_ptr<ImGui::IImGuiWindow> fpsWindow = std::make_unique<ImGui::FpsWindow>();

    Renderer renderer(vao, indexBuffer, program, pixelMap, imguiHandler);

    while (!window.ShouldClose()) {
        window.ProcessInput();
        imguiHandler.NewFrame();

        if (!beginWindow->ShouldClose([&]() { fluid = fluidBuilder.Build(); })) {
            beginWindow->Draw();
        } else {
#ifdef DEBUG
            fpsWindow->Draw();
#endif
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    fluid->AddDensity(pixelMap.GetWidth() / 2 + i, pixelMap.GetWidth() / 2 + j, Random::Int(150, 200));
                }
            }
            fluid->AddVelocity(
                pixelMap.GetWidth() / 2, pixelMap.GetWidth() / 2, Random::Double(-5, 5), Random::Double(-5, 5));

            fluid->Step();
        }

        renderer.Draw();
        window.SwapBuffers();
    }
    return 0;
}
