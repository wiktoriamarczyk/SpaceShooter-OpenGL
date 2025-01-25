#pragma once
#include "ModelObject.h"
#include "PointLight.h"

class Projectile : public ModelObject
{
public:
    void create(const glm::vec3& startPosition, const glm::vec3& targetPosition, TEAM team, float speed, const Model& model, const Shader& shader);
    void update(float deltaTime) override;
    TEAM getTeam() const { return team; }

private:
    shared_ptr<PointLight> light;
    glm::vec3 movementDirection;
    float speed = 0.f;
    TEAM team = TEAM::PLAYER;
};