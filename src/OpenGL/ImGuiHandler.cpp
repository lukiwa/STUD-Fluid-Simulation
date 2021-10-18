#include "ImGuiHandler.h"

/**
 * Initialize ImGui
 * @param windowHandler handler to GLFW window
 */
ImGui::Handler::Handler(GLFWwindow* windowHandler)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(windowHandler, true);
    std::string version = "#version 130";
    ImGui_ImplOpenGL3_Init(version.c_str());

    ImGui::StyleColorsLight();
}

/**
 * Deinitailize ImGui
 */
ImGui::Handler::~Handler()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/**
 * Render ImGui objects
 */
void ImGui::Handler::Draw() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 * Begin new ImGui frame
 */
void ImGui::Handler::NewFrame() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/**
 * Next ImGui window will be placed in the center of the display
 */
void ImGui::Handler::CenterNextWindow() const
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
}

/**
 * Draw Framerate window on screen
 */
void ImGui::FpsWindow::Draw()
{
    ImGui::Begin("Framerate");
    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
/**
 * FpsWindow never closes
 * @return false
 */
bool ImGui::FpsWindow::ShouldClose() const
{
    return false;
}

/**
 * Create new begining screen window (selection of parameters)
 * @param mainWindowWidth width of the root window
 * @param mainWindowHeight height of the root window
 * @param fluidBuilder builder for the fluid
 * @param pixelMapBuilder builder for the pixel map
 * @param handler ImGui handler
 */
ImGui::BeginWindow::BeginWindow(int mainWindowWidth, int mainWindowHeight, FluidBuilder& fluidBuilder,
    PixelMapBuilder& pixelMapBuilder, ImGui::Handler& handler)
    : _mainWindowWidth(mainWindowWidth)
    , _mainWindowHeight(mainWindowHeight)
    , _shouldClose(false)
    , _diffusion(0.0000001f)
    , _viscosity(0.00000001f)
    , _timestep(0.15f)
    , _iterations(10)
    , _simulationSize(128)
    , _automaticSimulation(false)
    , _fluidBuilder(fluidBuilder)
    , _pixelMapBuilder(pixelMapBuilder)
    , _handler(handler)
{
    _fluidBuilder.Size({ mainWindowWidth, mainWindowHeight, 0 });
}
/**
 * Draw begin window on the screen
 */
void ImGui::BeginWindow::Draw()
{
    if (!_shouldClose) {

        _handler.CenterNextWindow();
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        ImGui::Begin("Fluid Parameters", &_shouldClose,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::Text("Fluid Parameters: ");
        ImGui::SliderInt("Size", &_simulationSize, 16, _mainWindowWidth, "%d");
        ImGui::SliderInt("Gauss-Seidel iterations", &_iterations, 1, 20, "%d");
        ImGui::InputFloat("Diffusion", &_diffusion, 0.0000000001f, 0.000000001f, "%.10f");
        ImGui::InputFloat("Viscosity", &_viscosity, 0.0000000001f, 0.000000001f, "%.10f");
        ImGui::InputFloat("Timestep", &_timestep, 0.05f, 0.25f, "%.3f");
        ImGui::Checkbox("Automatic simulation", &_automaticSimulation);

        if (ImGui::Button("Start simulation", ImVec2(_mainWindowWidth / 2.0f, 0.0f))) {
            _shouldClose = true;
        }

        ImGui::End();
    }
}

/**
 * Check if window should now close
 * @param callback function to be called if window is closing
 * @return true if window is closing, false otherwise
 */
bool ImGui::BeginWindow::ShouldClose(std::function<void(void)> callback) const
{
    if (_shouldClose) {
        _fluidBuilder.Simulation()
            .Diffusion(_diffusion)
            .Viscosity(_viscosity)
            .TimeStep(_timestep)
            .Iterations(_iterations);
        _pixelMapBuilder.Size({ _simulationSize, _simulationSize, 0 });
        _fluidBuilder.Size({ _simulationSize, _simulationSize, 0 });
        _fluidBuilder.Visualization().IsAutomaticSimulation(_automaticSimulation);

        static bool called = false;
        if (!called) {
            called = true;
            callback();
        }
    }

    return _shouldClose;
}
