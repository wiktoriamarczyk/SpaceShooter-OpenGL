#pragma once
#include "Billboard.h"

class Star : public Billboard
{
public:
    void create(int ID, const Texture& texture, const Shader& shader, glm::vec3 cameraPosition);
    void update(float deltaTime) override;
    void render() override;
    int getID() const { return shaderArrayIndex; }
    void setID(int ID) { shaderArrayIndex = ID; }

private:
    int shaderArrayIndex;
    float speed = 5.0f;
};

