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
    float speed = 5.f;
    glm::vec2 movementDirection = glm::vec2(1.0f, 0.0f);
    float minX = -1.5f;                
    float maxX = 1.5f;                 
    float idleTime = 0.0f;             
    float targetX = 0.0f;
    float minDistance = 0.2f;
    //std::vector<shared_ptr<Projectile>> projectiles;
    //float shootCooldown;
    //const float shootInterval = 2.0f;
    //shared_ptr<Model> model2;
    //shared_ptr<Projectile> projectile;
    void setRandomIdleTime();
    //void shootProjectile();
    //glm::vec3 position;
    //shared_ptr<Shader> defaultModelShader;
};

