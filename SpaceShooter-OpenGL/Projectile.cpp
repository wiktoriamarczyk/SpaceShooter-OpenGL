#include "Projectile.h"

void Projectile::create(const glm::vec3& startPosition, const glm::vec3& direction, float speed, const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);
    position = startPosition;
    movementDirection = glm::normalize(direction);
    this->speed = speed;
}

void Projectile::update(float deltaTime)
{
    position += movementDirection * speed * deltaTime;
    if (isOffScreen())
        isObjectAlive = false;

    ModelObject::update(deltaTime);
}

bool Projectile::isOffScreen() const
{
    // Sprawdzenie, czy pocisk wyszed³ poza ekran (np. w osi Y lub X)
    return position.y > 1.0f || position.y < -1.0f || position.x > 1.0f || position.x < -1.0f;
}