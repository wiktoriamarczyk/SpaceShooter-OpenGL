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
    glm::vec3 targetPosition;  // Cel, do którego stacja ma siê przyci¹gaæ
    glm::vec3 startPosition;   // Pocz¹tkowa pozycja stacji
    float currentSpeed = 0.0f; // Prêdkoœæ poruszania siê stacji
    float acceleration = 0.5f; // Przyspieszenie stacji
    float decelerationDistance = 0.5f; // Odleg³oœæ do celu, gdzie prêdkoœæ zaczyna spadaæ
    float moveCooldown = 5.0f; // Czas, po którym stacja zacznie siê poruszaæ
    bool isAtCenter = false;   // Flaga, która wskazuje, ¿e stacja dotar³a do œrodka
    void moveToCenter(float deltaTime);
    void moveForward(float deltaTime);
    bool isPlayerNear(const glm::vec3& playerPosition);

};

