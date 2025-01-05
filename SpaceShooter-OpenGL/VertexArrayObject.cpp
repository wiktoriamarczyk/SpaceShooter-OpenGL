#include "VertexArrayObject.h"
#include "VertexBuffer.h"

bool VertexArrayObject::create(const VertexBuffer& VBO, VertexDefinitionElement elementFlags)
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);

    int size = 0;

    if ((elementFlags & POSITION) != 0)
    {
        size += 3 * sizeof(float);
    }
    if ((elementFlags & COLOR) != 0)
    {
        size += 3 * sizeof(float);
    }
    if ((elementFlags & TEXTURE_COORD) != 0)
    {
        size += 2 * sizeof(float);
    }

    int shift = 0;
    uint8_t* offset = nullptr;

    VBO.bind();

    if ((elementFlags & POSITION) != 0)
    {
        glVertexAttribPointer(int(VertexLayoutIndex::POSITION_INDEX), 3, GL_FLOAT, GL_FALSE, size, shift + offset);
        glEnableVertexAttribArray(0);
        shift += 3 * sizeof(float);
    }
    if ((elementFlags & COLOR) != 0)
    {
        glVertexAttribPointer(int(VertexLayoutIndex::COLOR_INDEX), 3, GL_FLOAT, GL_FALSE, size, shift + offset);
        glEnableVertexAttribArray(1);
        shift += 3 * sizeof(float);
    }
    if ((elementFlags & TEXTURE_COORD) != 0)
    {
        glVertexAttribPointer(int(VertexLayoutIndex::TEXTURE_COORD_INDEX), 2, GL_FLOAT, GL_FALSE, size, shift + offset);
        glEnableVertexAttribArray(2);
        shift += 2 * sizeof(float);
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