#pragma once
#include "ModelObject.h"

class Billboard : public ModelObject
{
public:
    void create(const Texture& texture, const Shader& shader, glm::vec3 cameraPosition);
    void update(float deltaTime) override;
    void render() override;

private:
    shared_ptr<Texture> texture;
    vector<float> vertices;
    glm::vec3 cameraPosition;
};

