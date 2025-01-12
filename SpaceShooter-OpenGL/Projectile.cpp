#include "Projectile.h"

void Projectile::create(const glm::vec3& startPosition, const glm::vec3& direction, float speed, const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);
    position = startPosition;
    movementDirection = glm::normalize(direction);
    this->speed = speed;
    setSize(glm::vec3(0.01f, 0.01f, 0.01f));  // Rozmiar domyœlny
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
    return position.y > 1.5f || position.y < -1.5f || position.x > 1.5f || position.x < -1.5f;
}

std::shared_ptr<Projectile> Projectile::createProjectile(const glm::vec3& startPosition, const glm::vec3& targetPosition, float speed, const Model& model, const Shader& shader)
{
    // Tworzymy pocisk w kierunku docelowej pozycji
    glm::vec3 direction = glm::normalize(targetPosition - startPosition);

    // Tworzymy nowy obiekt pocisku
    std::shared_ptr<Projectile> newProjectile = std::make_shared<Projectile>();
    newProjectile->create(startPosition, direction, speed, model, shader);
    return newProjectile;
}