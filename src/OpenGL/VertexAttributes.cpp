#include "VertexAttributes.h"
void VertexAttributes::Bind()
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
void VertexAttributes::AddAttribute(uint32_t count, GLenum type)
{
    VertexAttributeElement attribute = { count, type, GL_FALSE };
    _stride += count * GlTypeToSize(type);
    _attributes.push_back(attribute);
}

uint32_t VertexAttributes::GlTypeToSize(GLenum type)
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