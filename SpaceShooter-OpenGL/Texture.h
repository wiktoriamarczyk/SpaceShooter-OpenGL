#pragma once

#include "Common.h"

class Texture
{
public:
    static shared_ptr<Texture> create(const char* path);
    void bind(unsigned int slot = 0) const;
    void unbind(unsigned int slot) const;
    shared_ptr<Texture> getSelf() const { return self.lock(); }
    string getPath() const { return path; }
    ~Texture();

private:
    bool doCreate(const char* path);
    Texture() = default;
    unsigned int ID;
    int width, height, channels;
    string path;
    weak_ptr<Texture> self;
};

