//
// Created by shaux on 13.08.2021.
//

#include "Window.h"
#include <stdexcept>

namespace GLFW {

// needs to be a free function
void ResizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
 * Constructs a new glfw windows and context
 * @param width window width
 * @param height window height
 * @param title window title
 * @param glfwContextMajorVersion OpenGL major version (default 3)
 * @param glfwContextMinorVersion OpenGL minor version (default 3)
 */
Window::Window(int width, int height, const char* title, int glfwContextMajorVersion, int glfwContextMinorVersion)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwContextMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwContextMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (_windowHandle == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(_windowHandle);
    glfwSetFramebufferSizeCallback(_windowHandle, ResizeCallback);
}

Window::~Window()
{
    glfwTerminate();
}

/**
 * Should window close
 * @return true if should close false otherwise
 */
bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_windowHandle);
}

/**
 * Swap buffers
 */
void Window::SwapBuffers() const
{
    glfwSwapBuffers(_windowHandle);
}

/**
 * Handle inputs to a program
 */
void Window::ProcessInput()
{
    glfwPollEvents();

    if (glfwGetKey(_windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_windowHandle, true);
    }
}
}