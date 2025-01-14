#include "Star.h"

void Star::create(int ID, const Texture& texture, const Shader& shader, glm::vec3 cameraPosition)
{
    Billboard::create(texture, shader, cameraPosition);
}

void Star::update(float deltaTime)
{
    position.z += speed * deltaTime;

    Billboard::update(deltaTime);
}

void Star::render()
{
    Billboard::render();
}
