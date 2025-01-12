#include "Asteroid.h"

void Asteroid::create(const Model& model, const Shader& shader, const glm::vec3& startPos, const glm::vec3& playerPos) {

    ModelObject::create(model, shader);

    position = startPos;
    playerPosition = playerPos;
    setSize(glm::vec3(0.05f, 0.05f, 0.05f));

    direction = glm::normalize(playerPosition - position);
}

void Asteroid::update(float deltaTime) {
    if (!isObjectAlive) return;

    // Przesuwanie asteroidy w kierunku gracza
    position += direction * speed * deltaTime;

}

void Asteroid::render() {
    if (!isObjectAlive) return;
    ModelObject::render();
}