#include "VertexBuffer.h"

bool VertexBuffer::create(const void* vertices, unsigned int elementSize, unsigned int count)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, elementSize*count , vertices, GL_STATIC_DRAW);
    this->elementSize = elementSize;
    this->count = count;
    return true;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID);
}