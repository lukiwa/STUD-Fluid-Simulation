#pragma once

#include "../Tracing.h"
#include "GL/glew.h"
#include <algorithm>
#include <cstdint>
#include <list>

class VertexAttributes {
public:
    void Bind()
    {
        uint32_t offset = 0;
        for (std::size_t i = 0; i < _attributes.size(); ++i) {
            const auto& attribute = _attributes[i];
            GlAssert(glEnableVertexAttribArray(i));
            GlAssert(glVertexAttribPointer(
                i, attribute.count, attribute.type, attribute.normalized, _stride, (const void*)offset));
            offset += attribute.count * GlTypeToSize(attribute.type);
        }
    }
    /**
     * Do not normalize!!!
     * @param count
     * @param type
     */
    void AddAttribute(uint32_t count, GLenum type)
    {
        VertexAttributeElement attribute = { count, type, GL_FALSE };
        _stride += count * GlTypeToSize(type);
        _attributes.push_back(attribute);
    }

private:
    inline static uint32_t GlTypeToSize(GLenum type)
    {
        switch (type) {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_INT:
            return sizeof(GLint);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        default:
            LOG_WARNING("RETREVING SIZE OF UNEXPECTED TYPE!");
            return 0;
        }
    }

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
