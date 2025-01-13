#pragma once
#include "ModelObject.h"

class Projectile : public ModelObject
{
public:
    void create(const glm::vec3& startPosition, const glm::vec3& direction, float speed, const Model& model, const Shader& shader);
    void update(float deltaTime) override;
    static std::shared_ptr<Projectile> createProjectile(const glm::vec3& startPosition, const glm::vec3& targetPosition, float speed, const Model& model, const Shader& shader);


private:
    glm::vec3 movementDirection;
    float speed;
};