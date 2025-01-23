#pragma once
#include "ModelObject.h"

class Asteroid : public ModelObject
{
public:
    void create(const Model& model, const Shader& shader, const glm::vec3& playerPos);
    void update(float deltaTime) override;
    void render() override;
private:
    glm::vec3 direction;
};

