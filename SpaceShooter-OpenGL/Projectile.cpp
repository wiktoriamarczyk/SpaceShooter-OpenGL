#include "Projectile.h"
#include "Engine.h"

void Projectile::create(const glm::vec3& startPosition, const glm::vec3& targetPosition, float speed, const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);

    light = make_shared<PointLight>();
    Engine::getInstance().addPointLight(light);
    light->setParameters(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.0f, 1.0f, 1.0f), 0.22, 0.20);

    position = startPosition;
    this->speed = speed;
    offScreenDeathEnabled = true;
    movementDirection = glm::normalize(targetPosition - startPosition);
}

void Projectile::update(float deltaTime)
{
    position += movementDirection * speed * deltaTime;
    ModelObject::update(deltaTime);
}