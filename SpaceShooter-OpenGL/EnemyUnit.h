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
    float currentSpeed = 0.0f; // Aktualna prêdkoœæ, która roœnie od 0
    float acceleration = 1.0f; // Przyspieszenie (mo¿esz dostosowaæ wartoœæ)
    float decelerationDistance = 0.2f; // Odleg³oœæ, przy której zaczynamy zwalniaæ
    void setRandomIdleTime();
    void setRandomTargetPosition();
    void shootProjectile(const glm::vec3& playerPosition);
};

