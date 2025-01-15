#include "ChargingStation.h"

void ChargingStation::create(const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);
    rotation.x = 15.f;
    rotation.y = 0.f;

    setSize(glm::vec3(0.001f, 0.001f, 0.001f));

    // Pocz¹tkowa losowa pozycja stacji (np. g³êboko w przestrzeni)
    startPosition = glm::vec3(0.0f, -1.0f, -30.0f);
    position = startPosition;

    // Cel przyci¹gania (œrodek ekranu)
    targetPosition = glm::vec3(0.0f, -1.0f, -2.0f);

    // Ustawienie pocz¹tkowej prêdkoœci na 0
    currentSpeed = 0.0f;

    isAtCenter = false;
    moveCooldown = 5.0f;
}

void ChargingStation::update(float deltaTime)
{
    glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();

    if (!isAtCenter) {
        // Przyci¹ganie stacji do œrodka
        moveToCenter(deltaTime);
    }
    else {
        // Po dotarciu do œrodka, stacja czeka przez czas `moveCooldown` i potem rusza dalej
        if (moveCooldown > 0.0f) {
            moveCooldown -= deltaTime;  // Odliczamy czas oczekiwania
            if (isPlayerNear(Engine::getInstance().getPlayerPosition())) {
                std::cout << "Charging station: Leczenie w toku!" << std::endl; // Debug komunikat
            }

        }
        else {
            moveForward(deltaTime);  // Po odczekaniu, ruszamy do przodu
        }
    }

    ModelObject::update(deltaTime);
}

void ChargingStation::render()
{
    ModelObject::render();
}

void ChargingStation::moveToCenter(float deltaTime)
{
    // Obliczamy kierunek do œrodka
    glm::vec3 direction = glm::normalize(targetPosition - position);

    // Przemieszczamy stacjê w kierunku œrodka
    if (currentSpeed < 5.0f) {
        currentSpeed += acceleration * deltaTime;
        currentSpeed = glm::min(currentSpeed, 5.0f);
    }

    // Zmniejszamy prêdkoœæ w miarê zbli¿ania siê do œrodka
    float distanceToTarget = glm::distance(position, targetPosition);
    if (distanceToTarget < decelerationDistance) {
        currentSpeed = glm::max(currentSpeed * (distanceToTarget / decelerationDistance), 0.5f);
    }

    position += direction * currentSpeed * deltaTime;

    // Sprawdzamy, czy stacja dotar³a do œrodka
    if (distanceToTarget < 0.1f) {
        isAtCenter = true;
    }
}

void ChargingStation::moveForward(float deltaTime)
{
    // Kierunek poruszania siê stacji do przodu (wzd³u¿ osi Z)
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f); // Ruch w kierunku pozytywnej osi Z

    // Przemieszczamy stacjê do przodu
    position += direction * currentSpeed * deltaTime;
}

bool ChargingStation::isPlayerNear(const glm::vec3& playerPosition)
{
    // Sprawdzamy, czy gracz znajduje siê w pobli¿u stacji na osi X (w granicach +/- 1)
    return (playerPosition.x >= position.x - 1.0f && playerPosition.x <= position.x + 1.0f);
}