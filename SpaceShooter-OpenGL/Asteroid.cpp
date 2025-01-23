#include "Asteroid.h"

void Asteroid::create(const Model& model, const Shader& shader, const glm::vec3& direction) {

    ModelObject::create(model, shader);

    offScreenDeathEnabled = true;
    name = "Asteroid";
    this->direction = direction;
}

void Asteroid::update(float deltaTime)
{
    const float rotationFactor = randomFloat(10.0f, 30.0f);
    const float speed = randomFloat(1.0f, 5.0f);

    position.z += speed * deltaTime;
    rotation.x += rotationFactor * deltaTime;
    rotation.y += rotationFactor * deltaTime;

    ModelObject::update(deltaTime);
}

void Asteroid::render()
{
    ModelObject::render();
}