#include "Tracing.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdio.h>

#define WSL_ALLOW_DISPLAY setenv("DISPLAY", "127.0.0.1:0", true)

void ResizeCallback(GLFWwindow *window, int width, int height);

void ProcessInput(GLFWwindow *window);

static std::string FileToString(const std::string &filepath) {
    std::ifstream file(filepath);
    std::ostringstream oss;
    if (file.is_open()) {
        oss << file.rdbuf();
    } else {
        std::cerr << "ERROR OPENING FILE\n";
    }
    return oss.str();
}

static GLuint CompileShader(GLuint type, const std::string &source) {
    GLuint id = glCreateShader(type);
    const char *src_c_str = source.c_str();
    glShaderSource(id, 1, &src_c_str, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char message[1024];

        glGetShaderInfoLog(id, sizeof(message), nullptr, message);
        std::cerr << "FAILED TO COMPILE SHADER OF TYPE: " << type;
        std::cerr << " MESSAGE: \n"
                  << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        char message[512];
        glGetProgramInfoLog(program, sizeof(message), nullptr, message);
        std::cerr << "FAILED TO LINK SHADERS TO PROGRAM: ";
        std::cerr << "MESSAGE: \n"
                  << message << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(int, char **) {
    WSL_ALLOW_DISPLAY;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(512, 512, "LearnOpenGL", nullptr, nullptr);
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

    GLuint buffer;
    GlAssert(glGenBuffers(1, &buffer));
    GlAssert(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GlAssert(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GlAssert(glEnableVertexAttribArray(0));
    GlAssert(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
    GlAssert(glEnableVertexAttribArray(1));
    GlAssert(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float))));

    GLuint ibo;
    GlAssert(glGenBuffers(1, &ibo));
    GlAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GlAssert(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    auto vertexShader = FileToString("../shaders/vertex_shader.glsl");
    auto fragmentShader = FileToString("../shaders/fragment_shader.glsl");
    GLuint shader = CreateShader(vertexShader, fragmentShader);
    GlAssert(glUseProgram(shader));


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
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, 256 * 256 * 4, nullptr, GL_STREAM_DRAW);

    void *mappedBuffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    std::copy(std::begin(pixels), std::end(pixels), static_cast<GLubyte *>(mappedBuffer));
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);


    GLuint textureID;
    GlAssert(glGenTextures(1, &textureID));
    GlAssert(glBindTexture(GL_TEXTURE_2D, textureID));

    //GlAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GlAssert(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //GlAssert(glGenerateMipmap(GL_TEXTURE_2D));

    while (!glfwWindowShouldClose(window)) {
        ProcessInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        GlAssert(glClear(GL_COLOR_BUFFER_BIT));
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
        GlAssert(glBindTexture(GL_TEXTURE_2D, textureID));

        GlAssert(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }

    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    GlAssert(glDeleteProgram(shader));
    glfwTerminate();
    return 0;
}

void ResizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}