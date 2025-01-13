#include "Projectile.h"

void Projectile::create(const glm::vec3& startPosition, const glm::vec3& direction, float speed, const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);

    position = startPosition;
    movementDirection = glm::normalize(direction);
    this->speed = speed;
    offScreenDeathEnabled = true;
    setSize(glm::vec3(0.01f, 0.01f, 0.01f));
}

void Projectile::update(float deltaTime)
{
    position += movementDirection * speed * deltaTime;
    ModelObject::update(deltaTime);
}

std::shared_ptr<Projectile> Projectile::createProjectile(const glm::vec3& startPosition, const glm::vec3& targetPosition, float speed, const Model& model, const Shader& shader)
{
    glm::vec3 direction = glm::normalize(targetPosition - startPosition);

    std::shared_ptr<Projectile> newProjectile = std::make_shared<Projectile>();
    newProjectile->create(startPosition, direction, speed, model, shader);
    return newProjectile;
}