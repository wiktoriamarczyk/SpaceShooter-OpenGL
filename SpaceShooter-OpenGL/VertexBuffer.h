#pragma once

#include "Common.h"

class VertexBuffer
{
public:
    bool create(const void* vertices, unsigned int size);
    void bind() const;
    void unbind() const;
    uint32_t getSize() const { return size; }

    ~VertexBuffer();

private:
    unsigned int ID;
    uint32_t size = 0;
};

