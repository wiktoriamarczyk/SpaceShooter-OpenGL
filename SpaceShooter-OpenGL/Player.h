#pragma once
#include "ModelObject.h"
#include "Engine.h"

class Player : public ModelObject
{
public:
    void create(const Model& model, const Shader& shader, const Model& projectileModel);
    void update(float deltaTime) override;
    void render() override;
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;
    glm::vec3 getPosition() const;

private:
    float speed = 5.f;
    glm::vec3 movementDirection = glm::vec3(0.f);
    void shootProjectile(const glm::vec3& targetPosition);
    shared_ptr<Model> projectileModel;
    std::vector<shared_ptr<Projectile>> projectiles;
};

