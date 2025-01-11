#pragma once
#include "ModelObject.h"

class Projectile : public ModelObject
{
public:
    void create(const glm::vec3& startPosition, const glm::vec3& direction, float speed, const Model& model, const Shader& shader);
    void update(float deltaTime) override;
    bool isOffScreen() const;

private:
    glm::vec3 movementDirection;
    float speed;
};