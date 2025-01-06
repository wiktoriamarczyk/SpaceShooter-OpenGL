#include "VertexArrayObject.h"
#include "VertexBuffer.h"


VertexLayoutInfo VertexArrayObject::getLayoutInfo(VertexDefinitionElement elementFlags)
{
    VertexLayoutInfo info;
    int shift = 0;

    if ((elementFlags & POSITION) != 0)
    {
        int elementSize = 3 * sizeof(float);
        info.vertexBytes += elementSize;
        info.elements[info.elementsCount].index = VertexLayoutIndex::POSITION_INDEX;
        info.elements[info.elementsCount].normalized = false;
        info.elements[info.elementsCount].count = 3;
        info.elements[info.elementsCount].bytes = elementSize;
        info.elements[info.elementsCount].type = GL_FLOAT;
        info.elements[info.elementsCount++].offset = shift;
        shift += elementSize;
    }
    if ((elementFlags & COLOR) != 0)
    {
        int elementSize = 3 * sizeof(float);
        info.vertexBytes += elementSize;
        info.elements[info.elementsCount].index = VertexLayoutIndex::COLOR_INDEX;
        info.elements[info.elementsCount].normalized = true;
        info.elements[info.elementsCount].count = 3;
        info.elements[info.elementsCount].bytes = elementSize;
        info.elements[info.elementsCount].type = GL_FLOAT;
        info.elements[info.elementsCount++].offset = shift;
        shift += elementSize;
    }
    if ((elementFlags & TEXTURE_COORD) != 0)
    {
        int elementSize = 2 * sizeof(float);
        info.vertexBytes += elementSize;
        info.elements[info.elementsCount].index = VertexLayoutIndex::TEXTURE_COORD_INDEX;
        info.elements[info.elementsCount].normalized = false;
        info.elements[info.elementsCount].count = 2;
        info.elements[info.elementsCount].bytes = elementSize;
        info.elements[info.elementsCount].type = GL_FLOAT;
        info.elements[info.elementsCount++].offset = shift;
        shift += elementSize;
    }
    if ((elementFlags & NORMAL) != 0)
    {
        int elementSize = 3 * sizeof(float);
        info.vertexBytes += elementSize;
        info.elements[info.elementsCount].index = VertexLayoutIndex::NORMAL_INDEX;
        info.elements[info.elementsCount].normalized = true;
        info.elements[info.elementsCount].count = 3;
        info.elements[info.elementsCount].bytes = elementSize;
        info.elements[info.elementsCount].type = GL_FLOAT;
        info.elements[info.elementsCount++].offset = shift;
        shift += elementSize;
    }
    return info;
}

bool VertexArrayObject::create(const VertexBuffer& VBO, VertexDefinitionElement elementFlags)
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);

    VertexLayoutInfo layoutInfo = getLayoutInfo(elementFlags);

    VBO.bind();

    for (int i = 0; i < layoutInfo.elementsCount; i++)
    {
        const auto& element = layoutInfo.elements[i];
        glVertexAttribPointer(int(element.index), element.count, element.type, element.normalized, layoutInfo.vertexBytes, (void*)element.offset);
        glEnableVertexAttribArray(int(element.index));
    }

    return true;
}

void VertexArrayObject::bind() const
{
    glBindVertexArray(ID);
}

void VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &ID);
}