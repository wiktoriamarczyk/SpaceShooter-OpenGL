#pragma once
#include "ModelObject.h"
#include "Shader.h"
#include "Engine.h"

class ChargingStation : public ModelObject
{
public:
    void create(const Model& model, const Shader& shader);
    void update(float deltaTime);
    void render() override;

private:
    glm::vec3 targetPosition;  
    glm::vec3 startPosition;   
    float currentSpeed = 0.0f; 
    float acceleration = 0.5f; 
    float decelerationDistance = 0.5f; 
    float moveCooldown = 5.0f; 
    float timeOnScreen;
    bool isAtEdge = false; 
    void moveToEdge(float deltaTime);
    void moveBack(float deltaTime);
    float getRandomCooldown(float minCooldown, float maxCooldown);
    bool isPlayerNear(const glm::vec3& playerPosition);

};

