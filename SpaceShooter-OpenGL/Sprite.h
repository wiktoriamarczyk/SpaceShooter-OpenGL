#pragma once

#include "Common.h"
#include "Shader.h"

class Texture;
class VertexBuffer;
class IndexBuffer;
class VertexArrayObject;

class Sprite : public SelfHelper<Sprite>
{
    friend class SelfHelper<Sprite>;
public:
    static shared_ptr<Sprite> create(const Texture& Texture, const glm::vec2& position, const glm::vec2& size)
    {
        return SelfHelper<Sprite>::create(Texture, position, size);
    }
    void draw() const;
    void setPosition(const glm::vec2& position) { this->position = position; }
    void setSize(const glm::vec2& size) { this->size = size; }
    void setRotation(float rotation) { this->rotation = rotation; }
    void setColor(const glm::vec4& color) { this->color = color; }
    void setShader(const Shader& shader) { this->shader = shader.getSelf(); }
    const glm::vec2 getPosition() const { return position; }
    const glm::vec2 getSize() const { return size; }

    ~Sprite() = default;

protected:
    bool initialize(const Texture& Texture, const glm::vec2& position, const glm::vec2& size);
    Sprite() = default;

private:
    shared_ptr<VertexArrayObject> VAO;
    shared_ptr<VertexBuffer> VBO;
    shared_ptr<IndexBuffer> EBO;
    shared_ptr<Texture> texture;
    shared_ptr<Shader> shader;

    glm::vec2 position;
    glm::vec2 size;
    float rotation = 0.f;
    glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);

};

