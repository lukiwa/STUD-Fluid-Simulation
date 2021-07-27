#include "Tracing.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "OpenGL/IndexBuffer.h"
#include "OpenGL/ShaderProgram.h"
#include "OpenGL/VertexBuffer.h"

void ResizeCallback(GLFWwindow* window, int width, int height);

void ProcessInput(GLFWwindow* window);

int main(int, char**)
{
    ALLOW_DISPLAY;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(512, 512, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize OpenGL loader!\n";
        return -1;
    }

    GlAssert(glViewport(0, 0, 512, 512));
    GlAssert(glfwSetFramebufferSizeCallback(window, ResizeCallback));
    GlAssert(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    //create a scope, so all bound gl stuff will be unbind before glfw terminate
    {
        float positions[] = {
            // positions   // texture coords
            0.5f, 0.5f, 1.0f, 1.0f, // top right
            0.5f, -0.5f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f, 1.0f // top left
        };
        GLuint indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        GLuint VAO;
        GlAssert(glGenVertexArrays(1, &VAO));
        GlAssert(glBindVertexArray(VAO));

        VertexBuffer vertexBuffer(positions, sizeof(positions));

        GlAssert(glEnableVertexAttribArray(0));
        GlAssert(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
        GlAssert(glEnableVertexAttribArray(1));
        GlAssert(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));

        IndexBuffer indexBuffer(indices, 6);

        ShaderProgram program;
        program.ShaderFilename(GL_VERTEX_SHADER, "vertex_shader.glsl")
            .ShaderFilename(GL_FRAGMENT_SHADER, "fragment_shader.glsl")
            .CompileShaders()
            .LinkAndValidate()
            .Use();

        GLubyte pixels[256 * 256 * 4];
        for (int i = 0; i < sizeof(pixels); i++) {
            pixels[i] = 255;
        }

        for (int x = 0; x < 128; x++) {
            for (int y = 0; y < 128; y++) {
                pixels[4 * (x + y * 256) + 0] = 125;
                pixels[4 * (x + y * 256) + 1] = 125;
                pixels[4 * (x + y * 256) + 2] = 125;
                pixels[4 * (x + y * 256) + 3] = 125;
            }
        }

        GLuint pbo;
        GlAssert(glGenBuffers(1, &pbo));
        GlAssert(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo));
        GlAssert(glBufferData(GL_PIXEL_UNPACK_BUFFER, 256 * 256 * 4, nullptr, GL_STREAM_DRAW));

        void* mappedBuffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
        std::copy(std::begin(pixels), std::end(pixels), static_cast<GLubyte*>(mappedBuffer));
        GlAssert(glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER));

        GLuint textureID;
        GlAssert(glGenTextures(1, &textureID));
        GlAssert(glBindTexture(GL_TEXTURE_2D, textureID));

        // GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        // GL_LINEAR)); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        // GL_LINEAR_MIPMAP_NEAREST); glTexParameteri(GL_TEXTURE_2D,
        // GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        // GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE));
        GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        GlAssert(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        GlAssert(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
        GlAssert(glBindTexture(GL_TEXTURE_2D, 0));

        // GlAssert(glGenerateMipmap(GL_TEXTURE_2D));

        while (!glfwWindowShouldClose(window)) {
            ProcessInput(window);

            glfwSwapBuffers(window);
            glfwPollEvents();

            GlAssert(glClear(GL_COLOR_BUFFER_BIT));
            indexBuffer.Bind();

            GlAssert(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo));
            GlAssert(glBindTexture(GL_TEXTURE_2D, textureID));

            GlAssert(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        }

        program.Delete();
    }
    glfwTerminate();
    return 0;
}

void ResizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}