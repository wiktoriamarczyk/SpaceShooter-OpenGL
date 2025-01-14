#include "Asteroid.h"

void Asteroid::create(const Model& model, const Shader& shader, const glm::vec3& direction) {

    ModelObject::create(model, shader);
    offScreenDeathEnabled = true;
    this->direction = direction;
}

void Asteroid::update(float deltaTime)
{
    const float rotationFactor = 20.f;

    position.z += direction.z * speed * deltaTime;
    rotation.x += rotationFactor * deltaTime;
    rotation.y += rotationFactor * deltaTime;

    ModelObject::update(deltaTime);
}

void Asteroid::render()
{
    ModelObject::render();
}