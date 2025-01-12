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
    float speed = 2.f;
    /*glm::vec2 movementDirection = glm::vec2(1.0f, 0.0f);*/
    glm::vec3 movementDirection;
    glm::vec3 targetPosition;;
    const float minX = -1.0f; // Minimalna granica ruchu w osi X
    const float maxX = 1.0f;  // Maksymalna granica ruchu w osi X
    const float minY = -1.0f; // Minimalna granica ruchu w osi Y
    const float maxY = 1.0f;  // Maksymalna granica ruchu w osi Y
    float idleTime = 0.0f;
    float targetX = 0.0f;
    float minDistance = 0.2f;
    std::vector<shared_ptr<Projectile>> projectiles;
    float shootCooldown;
    const float shootInterval = 0.1f;
    float postShotIdleTime;
    shared_ptr<Model> projectileModel;
    int remainingShots;
    float postShotCooldown = 0.0f;
    //shared_ptr<Projectile> projectile;
    void setRandomIdleTime();
    void setRandomTargetPosition();
    void shootProjectile(const glm::vec3& playerPosition);
};

