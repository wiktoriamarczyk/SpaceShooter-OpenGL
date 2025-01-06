#pragma once
#include "GameObject.h"
#include "VertexBuffer.h";
#include "VertexArrayObject.h";
#include "Shader.h"

class Texture;

class TestCube : public GameObject
{
public:
    TestCube();
    void update(float deltaTime) override;
    void render() override;

private:
    VertexArrayObject VAO;
    VertexBuffer VBO;
    shared_ptr<Shader> shader;
    shared_ptr<Texture> texture;
};

