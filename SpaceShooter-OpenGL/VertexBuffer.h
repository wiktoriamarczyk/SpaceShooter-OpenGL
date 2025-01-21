#pragma once

#include "Common.h"

class VertexBuffer
{
public:
    bool create(const void* vertices, unsigned int elementSize, unsigned int count);
    void bind() const;
    void unbind() const;
    uint32_t getCount() const { return count; }

    ~VertexBuffer();

private:
    unsigned int ID;
    uint32_t elementSize = 0;
    uint32_t count = 0;
};

