#pragma once

#include "Common.h"

class VertexBuffer
{
public:
    bool create(const void* vertices, unsigned int elementSize, unsigned int count, int mode = GL_STATIC_DRAW);
    void bind() const;
    void unbind() const;
    uint32_t getCount() const { return count; }
    void updateData(const void* vertices, unsigned int elementSize, unsigned int count);

    ~VertexBuffer();

private:
    unsigned int ID;
    uint32_t elementSize = 0;
    uint32_t count = 0;
};

