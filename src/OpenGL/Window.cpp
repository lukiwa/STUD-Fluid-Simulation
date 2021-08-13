//
// Created by shaux on 13.08.2021.
//

#include "Window.h"
#include <stdexcept>

namespace GLFW {

// needs to be a free function
void ResizeCallback(GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); }

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
    glViewport(0, 0, width, height); // fullscren
}

Window::~Window() { glfwTerminate(); }
bool Window::ShouldClose() const { return glfwWindowShouldClose(_windowHandle); }
void Window::SwapBuffers() const { glfwSwapBuffers(_windowHandle); }
void Window::ProcessInput()
{
    glfwPollEvents();

    if (glfwGetKey(_windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_windowHandle, true);
    }
}
}