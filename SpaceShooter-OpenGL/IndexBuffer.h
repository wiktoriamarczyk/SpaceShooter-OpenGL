#pragma once

#include "Common.h"

class IndexBuffer
{
public:
    bool create(const unsigned int* indices, unsigned int size);
    void bind() const;
    void unbind() const;
    uint32_t getSize() const { return size; }

    ~IndexBuffer();

private:
    unsigned int ID;
    uint32_t size = 0;
};

