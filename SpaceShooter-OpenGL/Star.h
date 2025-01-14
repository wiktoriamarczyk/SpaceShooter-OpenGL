#pragma once
#include "Billboard.h"

class Star : public Billboard
{
public:
    void create(int index, const Texture& texture, const Shader& shader, glm::vec3 cameraPosition);
    void update(float deltaTime) override;
    void render() override;
    void updateShaderData();
    int getIndex() const { return shaderArrayIndex; }
    void setIndex(int index);

private:
    int shaderArrayIndex;
    float speed = 5.0f;
};

