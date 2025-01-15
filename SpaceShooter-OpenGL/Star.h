#pragma once
#include "Billboard.h"
#include "PointLight.h"

class Star : public Billboard
{
public:
    void create(const Texture& texture, const Shader& shader, glm::vec3 cameraPosition);
    void update(float deltaTime) override;
    void render() override;

private:
    shared_ptr<PointLight> light;
    float speed = 5.0f;
};

