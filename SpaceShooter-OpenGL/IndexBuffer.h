#pragma once

#include "Common.h"

class IndexBuffer
{
public:
    bool create(const unsigned int* indices, bool is32Bit, unsigned int count);
    void bind() const;
    void unbind() const;
    uint32_t getCount() const { return count; }

    ~IndexBuffer();

private:
    unsigned int ID;
    uint32_t is32Bit = 0;
    uint32_t count = 0;
};

