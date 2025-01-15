#include "Star.h"
#include "Engine.h"

void Star::create(const Texture& texture, const Shader& shader, glm::vec3 cameraPosition)
{
    offScreenDeathEnabled = true;
    name = "Star";

    light = make_shared<PointLight>();
    Engine::getInstance().addPointLight(light);

    Billboard::create(texture, shader, cameraPosition);
}

void Star::update(float deltaTime)
{
    // Move star towards camera
    position.z += speed * deltaTime;

    light->updateLightPosition(position);
    light->updateLightParameters();

    Billboard::update(deltaTime);
}

void Star::render()
{
    light->setAdditiveBlending(true);

    Billboard::render();

    light->setAdditiveBlending(false);
}
