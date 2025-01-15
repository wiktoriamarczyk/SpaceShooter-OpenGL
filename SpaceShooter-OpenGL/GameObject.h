#pragma once
#include "Common.h"
#include "Sprite.h"

class GameObject
{
public:
    GameObject() = default;
    virtual void update(float deltaTime);
    virtual void render() = 0;
    virtual void onKeyDown(int key) {};
    virtual void onKeyUp(int key) {};
    void setPosition(const glm::vec3& position) { this->position = position; }
    void setSize(const glm::vec3& size) { this->size = size; }
    void setRotation(const glm::vec3& rotation) { this->rotation = rotation; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getSize() const { return size; }
    glm::vec3 getRotation() const { return rotation; }
    bool isAlive() const { return alive; }
    bool isOffScreen() const;
    void setScreenBoundZ(float z) { screenBoundZ = z; }
    virtual ~GameObject() = default;

protected:
    bool alive = true;
    bool offScreenDeathEnabled = false;
    glm::vec2 screenBoundsX = glm::vec2(-1.5f, 1.5f);
    glm::vec2 screenBoundsY = glm::vec2(-1.5f, 1.5f);
    float screenBoundZ = 2.0f;
    glm::vec3 position;
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation;
    string name = "GameObject";
};

