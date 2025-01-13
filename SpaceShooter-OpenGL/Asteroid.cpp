#include "Asteroid.h"

void Asteroid::create(const Model& model, const Shader& shader, const glm::vec3& startPos, const glm::vec3& direction) {

    ModelObject::create(model, shader);

    position = startPos;
    setSize(glm::vec3(0.05f, 0.05f, 0.05f));
    offScreenDeathEnabled = true;
    this->direction = direction;
}

void Asteroid::update(float deltaTime)
{
    position.z += direction.z * speed * deltaTime;
    rotation.x += 20.f * deltaTime;
    rotation.y += 20.f * deltaTime;
}

void Asteroid::render() {
    ModelObject::render();
}