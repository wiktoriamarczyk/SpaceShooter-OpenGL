#include "IndexBuffer.h"

bool IndexBuffer::create(const unsigned int* indices, bool is32Bit, unsigned int count)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*(is32Bit?4:2), indices, GL_STATIC_DRAW);
    this->is32Bit = is32Bit;
    this->count = count;
    return true;
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &ID);
}
