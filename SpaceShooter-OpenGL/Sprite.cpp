#include "Sprite.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArrayObject.h"
#include "Engine.h"

bool Sprite::initialize(const Texture& texture, const glm::vec2& position, const glm::vec2& size)
{
    this->texture = texture.getSelf();
    this->position = position;
    this->size = size;

    VAO = Engine::GetDefaultVAO();
    VBO = Engine::GetDefaultVBO();
    EBO = Engine::GetDefaultIBO();
    shader = Engine::GetDefaultSpriteShader();

    return true;
}

void Sprite::draw() const
{
    if (!texture || !shader)
        return;

    if (!VAO)
        return;

    texture->bind();
    VAO->bind();
    shader->use();

    glm::mat4 model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader->setMat4("model", model);

    if (EBO)
    {
        EBO->bind();
        glDrawElements(GL_TRIANGLES, EBO->getCount(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, VBO->getCount());
    }
}
