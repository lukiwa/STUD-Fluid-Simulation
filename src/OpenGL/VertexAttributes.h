#pragma once

#include "Tracing.h"
#include "GL/glew.h"
#include <algorithm>
#include <cstdint>
#include <list>

class VertexAttributes {
public:
    VertexAttributes();
    ~VertexAttributes() = default;
    void Bind();

    void AddAttribute(uint32_t count, GLenum type);

    static uint32_t GlTypeToSize(GLenum type);

private:
    struct VertexAttributeElement {
        uint32_t count;
        GLenum type;
        GLboolean normalized = GL_FALSE;
    };

    // I do not actually need to store it on the CPU side, but may be useful to debug
    std::vector<VertexAttributeElement> _attributes;
    uint32_t _stride;
};
