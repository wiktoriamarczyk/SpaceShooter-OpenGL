#pragma once

#include "Common.h"

class Texture;
class VertexBuffer;
class IndexBuffer;
class Shader;
class VertexArrayObject;

class Sprite
{
public:
    void create(const Texture& Texture, const glm::vec2& position, const glm::vec2& size);
    void draw() const;

    void setPosition(const glm::vec2& position) { this->position = position; }
    void setSize(const glm::vec2& size) { this->size = size; }
    void setRotation(float rotation) { this->rotation = rotation; }

    ~Sprite() = default;

private:
    shared_ptr<VertexBuffer> VBO;
    shared_ptr<IndexBuffer> EBO;
    shared_ptr<VertexArrayObject> VAO;
    shared_ptr<Shader> shader;
    shared_ptr<Texture> texture;

    glm::vec2 position;
    glm::vec2 size;
    float rotation = 0.f;
    glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);

};

