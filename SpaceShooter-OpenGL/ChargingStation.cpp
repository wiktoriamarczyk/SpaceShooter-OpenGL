#include "ChargingStation.h"

void ChargingStation::create(const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);
    rotation.x = 0.f;
    rotation.y = 130.f;

    setSize(glm::vec3(0.045f, 0.045f, 0.045f));

    startPosition = glm::vec3(3.0f, 0.0f, -2.0f);
    position = startPosition;
    position.z = -3.5f;

    currentSpeed = 0.0f;

    isAtEdge = false;

    srand(static_cast<unsigned int>(time(0)));

    moveCooldown = getRandomCooldown(60.0f, 150.0f);
    timeOnScreen = getRandomCooldown(5.0f, 10.0f);
}

void ChargingStation::update(float deltaTime)
{
    glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();

    if (isPlayerNear(playerPosition)) {
        std::cout << "Player is near the charging station!" << std::endl;
    }

    if (!isAtEdge) {
        if (moveCooldown > 0.0f) {
            moveCooldown -= deltaTime;
        }
        else {
            moveToEdge(deltaTime);
        }
    }
    else {
        position.y = glm::mix(position.y, playerPosition.y, deltaTime * 2.0f);

        if (timeOnScreen > 0.0f) {
            timeOnScreen -= deltaTime;
        }
        else {
            moveBack(deltaTime);
        }
    }

    ModelObject::update(deltaTime);
}

void ChargingStation::render()
{
    ModelObject::render();
}

void ChargingStation::moveToEdge(float deltaTime)
{
    glm::vec3 targetPosition = glm::vec3(1.0f, position.y, position.z);
    glm::vec3 direction = glm::normalize(targetPosition - position);

    if (currentSpeed < 5.0f) {
        currentSpeed += acceleration * deltaTime;
        currentSpeed = glm::min(currentSpeed, 5.0f);
    }

    position += direction * currentSpeed * deltaTime;

    if (glm::distance(position, targetPosition) < 0.1f) {
        isAtEdge = true;
        currentSpeed = 0.0f;

        timeOnScreen = getRandomCooldown(5.0f, 10.0f);
    }
}

void ChargingStation::moveBack(float deltaTime)
{
    glm::vec3 direction = glm::normalize(startPosition - position);

    if (currentSpeed < 5.0f) {
        currentSpeed += acceleration * deltaTime;
        currentSpeed = glm::min(currentSpeed, 5.0f);
    }

    position += direction * currentSpeed * deltaTime;

    if (glm::distance(position, startPosition) < 0.1f) {
        isAtEdge = false;
        currentSpeed = 0.0f;

        moveCooldown = getRandomCooldown(60.0f, 150.0f);
    }
}

float ChargingStation::getRandomCooldown(float minCooldown, float maxCooldown)
{
    return minCooldown + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxCooldown - minCooldown)));
}

bool ChargingStation::isPlayerNear(const glm::vec3& playerPosition)
{
    return glm::abs(playerPosition.x - position.x) <= 0.8f;
}