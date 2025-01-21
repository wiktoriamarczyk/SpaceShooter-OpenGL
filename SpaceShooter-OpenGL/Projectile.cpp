#include "Projectile.h"
#include "Engine.h"

void Projectile::create(const glm::vec3& startPosition, const glm::vec3& targetPosition, float speed, const Model& model, const Shader& shader)
{
    ModelObject::create(model, *Engine::GetDefaultLightShader());

    light = make_shared<PointLight>();
    Engine::getInstance().addPointLight(light);
    light->setParameters(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 1.4, 2.8);

    position = startPosition;
    this->speed = speed;
    offScreenDeathEnabled = true;
    movementDirection = glm::normalize(targetPosition - startPosition);
}

void Projectile::update(float deltaTime)
{
    position += movementDirection * speed * deltaTime;
    ModelObject::update(deltaTime);

    light->updateLightPosition(position);
    light->updateLightParameters();
}