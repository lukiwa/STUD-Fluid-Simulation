#include "Utilities/Tracing.h"
#include <fstream>
#include <iostream>
#include <memory>

#include "OpenGL/IndexBuffer.h"
#include "OpenGL/PixelBufferObject.h"
#include "OpenGL/PixelMap.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/VertexAttributes.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/Window.h"
#include "Utilities/Random.h"

int main(int, char**)
{
    ALLOW_DISPLAY;
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

    PixelMap pixelMap({ 512, 512, 0 }, GL_RGBA, new PixelMapComponentsFactory());
    pixelMap.SetAllPixels({ 255, 255, 255, 255 });
    pixelMap.SwapBuffer();

    Renderer renderer(vao, indexBuffer, program, pixelMap);

    while (!window.ShouldClose()) {
        window.ProcessInput();

        pixelMap.SetPixel(Random::Int(0, 512), Random::Int(0, 512),
              { Random::Int(0, 255), Random::Int(0, 255), Random::Int(0, 255), 255 });


        renderer.Draw();

        window.SwapBuffers();
    }
    return 0;
}
