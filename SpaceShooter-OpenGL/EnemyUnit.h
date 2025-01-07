#pragma once
#include "ModelObject.h"

class EnemyUnit : public ModelObject
{
public:
    void create(const Model& model, const Shader& shader) override;
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

    void setRandomIdleTime();
};

