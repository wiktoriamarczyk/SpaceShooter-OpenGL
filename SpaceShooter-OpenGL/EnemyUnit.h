#pragma once
#include "ModelObject.h"
#include "Projectile.h"
#include "Model.h"

class EnemyUnit : public ModelObject
{
public:
    void create(const Model& model, const Shader& shader, const Model& projectileModel);
    void update(float deltaTime) override;
    void render() override;
private:
    shared_ptr<Model> projectileModel;

    float speed = 2.f;
    glm::vec3 movementDirection;
    glm::vec3 targetPosition;;
    const float minX = -5.0f;
    const float maxX = 5.0f;
    const float minY = -5.0f;
    const float maxY = 5.0f;
    float idleTime = 0.0f;
    float targetX = 0.0f;
    float minDistance = 0.5f;
    float shootCooldown;
    const float shootInterval = 0.1f;
    float postShotIdleTime;
    int remainingShots;
    float postShotCooldown = 0.0f;
    float currentSpeed = 0.0f;
    float acceleration = 1.0f;
    float decelerationDistance = 0.2f;

    void setRandomIdleTime();
    void setRandomTargetPosition();
    void shootProjectile(const glm::vec3& playerPosition);
};

