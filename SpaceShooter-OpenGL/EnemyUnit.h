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
    glm::vec2 movementDirection = glm::vec2(1.0f, 0.0f);
    const float minX = -1.5f;
    const float maxX = 1.5f;
    float idleTime = 0.0f;
    float targetX = 0.0f;
    float minDistance = 0.2f;
    std::vector<shared_ptr<Projectile>> projectiles;
    float shootCooldown;
    const float shootInterval = 1.0f;
    shared_ptr<Model> projectileModel;
    //shared_ptr<Projectile> projectile;
    void setRandomIdleTime();
    void shootProjectile();
};

