#include "VertexBuffer.h"

bool VertexBuffer::create(const void* vertices, unsigned int elementSize, unsigned int count, int mode)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, elementSize*count , vertices, mode);
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

void VertexBuffer::updateData(const void* vertices, unsigned int elementSize, unsigned int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, elementSize * count, vertices);
    this->elementSize = elementSize;
    this->count = count;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID);
}