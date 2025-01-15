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
    glm::vec3 targetPosition;  // Cel, do kt�rego stacja ma si� przyci�ga�
    glm::vec3 startPosition;   // Pocz�tkowa pozycja stacji
    float currentSpeed = 0.0f; // Pr�dko�� poruszania si� stacji
    float acceleration = 0.5f; // Przyspieszenie stacji
    float decelerationDistance = 0.5f; // Odleg�o�� do celu, gdzie pr�dko�� zaczyna spada�
    float moveCooldown = 5.0f; // Czas, po kt�rym stacja zacznie si� porusza�
    bool isAtCenter = false;   // Flaga, kt�ra wskazuje, �e stacja dotar�a do �rodka
    void moveToCenter(float deltaTime);
    void moveForward(float deltaTime);
    bool isPlayerNear(const glm::vec3& playerPosition);

};

