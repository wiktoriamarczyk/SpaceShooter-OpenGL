#pragma once
#include "Common.h"
#include "Sprite.h"

class GameObject
{
public:
    GameObject() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void onKeyDown(int key) {};
    virtual void onKeyUp(int key) {};
    virtual ~GameObject() = default;

protected:
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotation;
    Sprite sprite;
};

