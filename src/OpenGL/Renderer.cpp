#include "Renderer.h"

/**
 * Create new renderer
 * @param vao vertex array object bound with VertexAttrubutes and VertexBuffer
 * @param ibo index buffer bound with vertices
 * @param program compiled and validated shader program
 * @param pixelMap
 */
Renderer::Renderer(VertexArray& vao, IndexBuffer& ibo, ShaderProgram& program, PixelMap& pixelMap)
    : _vao(vao)
    , _ibo(ibo)
    , _program(program)
    , _pixelMap(pixelMap)
{
    _program.Use();
}

/**
 * Bind components and issue draw call
 */
void Renderer::Draw() const
{
    _vao.Bind();
    _ibo.Bind();
    _program.Use();
    _pixelMap.Bind();
    _pixelMap.SwapBuffer();


    GlAssert(glDrawElements(GL_TRIANGLES, _ibo.Count(), GL_UNSIGNED_INT, nullptr));
}

/**
 * Set screen clearcolor
 * @param components components of the background color
 * NOTE: Have to have size 4 - RGBA
 */
void Renderer::ClearColor(const std::vector<int>& components) const
{
    assert(components.size() == 4);
    GlAssert(glClearColor(components[0], components[1], components[2], components[3]));
}

/**
 * Clear screen
 */
void Renderer::Clear() const
{
    _pixelMap.Clear();
    GlAssert(glClear(GL_COLOR_BUFFER_BIT));
}
