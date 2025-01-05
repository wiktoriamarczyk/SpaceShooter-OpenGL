#pragma once

#include "Common.h"

class VertexBuffer;

class VertexArrayObject
{
public:
    enum VertexDefinitionElement
    {
        POSITION = 1 << 0,
        COLOR = 1 << 1,
        TEXTURE_COORD = 1 << 2
    };

    enum class VertexLayoutIndex : uint8_t
    {
        POSITION_INDEX = 0,
        COLOR_INDEX = 1,
        TEXTURE_COORD_INDEX = 2
    };

    bool create(const VertexBuffer& VBO, VertexDefinitionElement elementFlags);
    void bind() const;
    void unbind() const;

    ~VertexArrayObject();

private:
    unsigned int ID;
};

inline VertexArrayObject::VertexDefinitionElement operator|(VertexArrayObject::VertexDefinitionElement a, VertexArrayObject::VertexDefinitionElement b)
{
    return static_cast<VertexArrayObject::VertexDefinitionElement>(static_cast<int>(a) | static_cast<int>(b));
}