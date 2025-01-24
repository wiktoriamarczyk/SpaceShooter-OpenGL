#pragma once
#include "ModelObject.h"
#include "Engine.h"
#include "HealthBar.h"

class Player : public ModelObject
{
public:
    void create(const Model& model, const Shader& shader, const Model& projectileModel);
    void update(float deltaTime) override;
    void render() override;
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;
    void onMouseButtonDown(int button) override;
    glm::vec3 getPosition() const;
    void updateHealth(float value) override;

private:
    float speed = 3.f;
    glm::vec3 movementDirection = glm::vec3(0.f);
    shared_ptr<Model> projectileModel;
    shared_ptr<HealthBar> healthBar;

    void shootProjectile(const glm::vec3& targetPosition);
    void move(float deltaTime);
    void tilt(float deltaTime);
    void rotateTheHead(float deltaTime);
};

