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
    void setPosition(const glm::vec3& position) { this->position = position; }
    void setSize(const glm::vec3& size) { this->size = size; }
    void setRotation(const glm::vec3& rotation) { this->rotation = rotation; }
    void setSprite(const Sprite& sprite) { this->sprite = sprite.getSelf(); }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getSize() const { return size; }
    glm::vec3 getRotation() const { return rotation; }
    shared_ptr<Sprite> getSprite() const { return sprite; }
    virtual ~GameObject() = default;

protected:
    glm::vec3 position;
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation;
    shared_ptr<Sprite> sprite;
};
