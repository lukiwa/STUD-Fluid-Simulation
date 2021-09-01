#pragma once
#include "../Simulation/FluidBuilder.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <functional>

namespace ImGui {

class IImGuiWindow {
public:
    virtual ~IImGuiWindow() = default;
    virtual void Draw() = 0;
    virtual bool ShouldClose() const
    {
        return false;
    }
    virtual bool ShouldClose(std::function<void(void)> callback) const
    {
        callback();
        return false;
    }
};

class Handler {
public:
    explicit Handler(GLFWwindow* windowHandler);

    ~Handler();
    void Draw() const;

    void NewFrame() const;

    void CenterNextWindow() const;

    void FramerateWindow() const;
};

class FpsWindow : public IImGuiWindow {
public:
    void Draw() override;

    bool ShouldClose() const override;
};


class BeginWindow : public IImGuiWindow {
public:
    BeginWindow(int mainWindowWidth, FluidBuilder& builder, ImGui::Handler& handler);
    void Draw() override;
    bool ShouldClose(std::function<void(void)> callback) const override;

private:
    int _mainWindowWidth;
    bool _shouldClose;
    float _diffusion;
    float _viscosity;
    float _timestep;
    FluidBuilder& _builder;
    ImGui::Handler& _handler;
};
}