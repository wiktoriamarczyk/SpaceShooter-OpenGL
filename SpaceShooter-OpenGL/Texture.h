#pragma once

#include "Common.h"

class Texture : public SelfHelper<Texture>
{
    friend class SelfHelper<Texture>;
public:
    static shared_ptr<Texture> create(const char* path, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE)
    {
        return SelfHelper<Texture>::create(path, type);
    }
    static shared_ptr<Texture> create(int width, int height)
    {
        return SelfHelper<Texture>::create(width, height);
    }
    void bind(unsigned int slot = 0) const;
    void attachToFramebuffer(unsigned int frmaebuffferID) const;
    void unbind(unsigned int slot) const;
    string getPath() const { return path; }
    aiTextureType getType() const { return type; }
    ~Texture();
private:
    Texture() = default;
    bool initialize(const char* path, aiTextureType type);
    bool initialize(int width, int height);
    unsigned int ID;
    int width, height, channels;
    string path;
    aiTextureType type = aiTextureType::aiTextureType_DIFFUSE;
};