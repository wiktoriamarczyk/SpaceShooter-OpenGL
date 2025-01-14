#include "Billboard.h"
#include "Engine.h"
#include "Shader.h"

void Billboard::create(const Texture& texture, const Shader& shader, glm::vec3 cameraPosition)
{
    this->texture = texture.getSelf();
    this->cameraPosition = cameraPosition;
    this->offScreenDeathEnabled = true;
    ModelObject::create(texture, shader);
}

void Billboard::update(float deltaTime)
{
    // rotate towards target
    glm::vec3 direction = cameraPosition - position;
    direction = glm::normalize(direction);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat rotationQuat = glm::quatLookAt(-direction, up);
    rotation = glm::degrees(glm::eulerAngles(rotationQuat));

    ModelObject::update(deltaTime);
}

void Billboard::render()
{
    ModelObject::render();
}
