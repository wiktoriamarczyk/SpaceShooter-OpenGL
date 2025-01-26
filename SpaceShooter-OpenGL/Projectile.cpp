#include "Projectile.h"
#include "Engine.h"

void Projectile::create(const glm::vec3& startPosition, const glm::vec3& targetPosition, TEAM team, float speed, const Model& model)
{
    ModelObject::create(model, *Engine::GetDefaultEmissiveShader());

    light = make_shared<PointLight>();
    Engine::getInstance().addPointLight(light);
    light->setParameters(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.0f, 0.0f, 0.0f), 1.4, 2.8);
    this->speed = speed;
    this->team = team;
    this->color = team == TEAM::PLAYER ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
    position = startPosition;
    offScreenDeathEnabled = true;
    movementDirection = glm::normalize(targetPosition - startPosition);
}

void Projectile::update(float deltaTime)
{
    position += movementDirection * speed * deltaTime;
    rotation.x += randomFloat(30.0f, 60.0f) * deltaTime;
    rotation.y += randomFloat(30.0f, 60.0f) * deltaTime;

    ModelObject::update(deltaTime);

    light->updateLightPosition(position);
    light->updateLightParameters();
}

void Projectile::render()
{
    ModelObject::render();
}
