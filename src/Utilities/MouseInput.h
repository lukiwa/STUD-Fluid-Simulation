#pragma once
#include "Window.h"
#include <functional>

class MouseInput {
public:
    MouseInput(GLFWwindow* handle);

    MouseInput(int textureWidth, int textureHeight, int windowWidth, int windowHeight, GLFWwindow* handle);
    bool CheckMouseButton(int button, int result);

    std::pair<int, int> MappedMousePosition(bool yAxisReverse = true);

    std::pair<double, double> MousePosition();

    void WheelCallback(std::function<void(bool)> wheelCallback);

    static int Map(int x, int inMin, int inMax, int outMin, int outMax);

private:
    double _x;
    double _y;
    int _xMapped;
    int _yMapped;

    int _windowWidth;
    int _windowHeight;
    int _textureWidth;
    int _textureHeight;

    bool _allowMapped;
    GLFWwindow* _handle;
    std::function<void(bool)> _wheelCallback;
};