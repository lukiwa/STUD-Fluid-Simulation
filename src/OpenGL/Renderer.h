#pragma once

#include "Tracing.h"
#include "IndexBuffer.h"
#include "PixelMap.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include <vector>
#include "ImGuiHandler.h"

class Renderer {
public:
    Renderer(VertexArray& vao, IndexBuffer& ibo, ShaderProgram& program, PixelMap* pixelMap, ImGui::Handler& imguiHandler);
    ~Renderer() = default;

    void Draw() const;

    void SetPixelMap(PixelMap* pixelMap);

    void ClearColor(const std::vector<int>& components) const;

    void Clear() const;

private:
    VertexArray& _vao;
    IndexBuffer& _ibo;
    ShaderProgram& _program;
    PixelMap* _pixelMap;
    ImGui::Handler& _imguiHandler;
};
