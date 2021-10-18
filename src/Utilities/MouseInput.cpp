#include "MouseInput.h"
#include <cassert>
#include <iostream>

/**
 * Construct new mouse input handler but does not allow mapping values to texture inside window
 * @param handle window handle
 */
MouseInput::MouseInput(GLFWwindow* handle)
    : _allowMapped(false)
    , _handle(handle)
{
}

/**
 * Creates new mouse input and allow mapping values
 * @param textureWidth
 * @param textureHeight
 * @param windowWidth
 * @param windowHeight
 * @param handle
 */
MouseInput::MouseInput(int textureWidth, int textureHeight, int windowWidth, int windowHeight, GLFWwindow* handle)
    : _windowWidth(windowWidth)
    , _windowHeight(windowHeight)
    , _textureWidth(textureWidth)
    , _textureHeight(textureHeight)
    , _allowMapped(true)
    , _handle(handle)
{
}

/**
 * Check for button state
 * @param button mouse button
 * @param result button state
 * @return true if real state equals result
 */
bool MouseInput::CheckMouseButton(int button, int result)
{
    return glfwGetMouseButton(_handle, button) == result;
}
/**
 * Get Mouse position and map it
 * @param yAxisReverse should yAxis be reversed (for example when using pixelMap as texture)
 * @return mapped mouse position
 */
std::pair<int, int> MouseInput::MappedMousePosition(bool yAxisReverse)
{
    assert(_allowMapped);
    glfwGetCursorPos(_handle, &_x, &_y);
    _xMapped = Map(_x, 0, _windowWidth, 0, _textureWidth);
    if (yAxisReverse) {
        _yMapped = _textureHeight - Map(_y, 0, _windowHeight, 0, _textureHeight);
    }
    return { _xMapped, _yMapped };
}
/**
 * Get mouse position
 * @return mouse position
 */
std::pair<double, double> MouseInput::MousePosition()
{
    glfwGetCursorPos(_handle, &_x, &_y);
    return { _x, _y };
}

/**
 * Map values
 * @param x value to be mapped
 * @param inMin
 * @param inMax
 * @param outMin
 * @param outMax
 * @return mapped value
 */
int MouseInput::Map(int x, int inMin, int inMax, int outMin, int outMax)
{
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/**
 * Set callback to be called on mouse wheel input
 * @param wheelCallback function to be called on mouse input
 */
void MouseInput::WheelCallback(std::function<void(bool)> wheelCallback)
{
    _wheelCallback = wheelCallback;
    glfwSetWindowUserPointer(_handle, this);

    glfwSetScrollCallback(_handle, [](GLFWwindow* handle, double, double offsetY) {
        auto self = static_cast<MouseInput*>(glfwGetWindowUserPointer(handle));
        assert(self != nullptr);

        self->_wheelCallback(offsetY == 1);
    });
}
