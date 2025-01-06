#pragma once

#include "Common.h"

class VertexBuffer;

enum VertexDefinitionElement
{
    POSITION = 1 << 0,
    COLOR = 1 << 1,
    TEXTURE_COORD = 1 << 2,
    NORMAL = 1 << 3,

    __COUNT = 4
};

enum class VertexLayoutIndex : uint8_t
{
    POSITION_INDEX = 0,
    COLOR_INDEX = 1,
    TEXTURE_COORD_INDEX = 2,
    NORMAL_INDEX = 3
};

struct VertexLayoutInfo
{
    static constexpr int MAX_ELEMENTS = static_cast<int>(VertexDefinitionElement::__COUNT);
    struct Element
    {
        VertexLayoutIndex index = VertexLayoutIndex::POSITION_INDEX;
        int offset = 0;
        int count = 0;
        int bytes = 0;
        int type = 0;
        bool normalized = false;
    };
    int vertexBytes = 0;
    int elementsCount = 0;
    Element elements[MAX_ELEMENTS];
};

class VertexArrayObject
{
public:
    static VertexLayoutInfo getLayoutInfo(VertexDefinitionElement elementFlags);
    bool create(const VertexBuffer& VBO, VertexDefinitionElement elementFlags);
    void bind() const;
    void unbind() const;

    ~VertexArrayObject();

private:
    unsigned int ID;
};

inline VertexDefinitionElement operator|(VertexDefinitionElement a, VertexDefinitionElement b)
{
    return static_cast<VertexDefinitionElement>(static_cast<int>(a) | static_cast<int>(b));
}

inline VertexDefinitionElement operator|= (VertexDefinitionElement & a, VertexDefinitionElement b)
{
    return a = a | b;
}