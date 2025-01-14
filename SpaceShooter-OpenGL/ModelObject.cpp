#include "ModelObject.h"
#include "Model.h"
#include "Shader.h"
#include "Engine.h"

void ModelObject::create(const Model& model, const Shader& shader)
{
    this->model = model.getSelf();
    this->shader = shader.getSelf();
}

void ModelObject::create(const Texture& texture, const Shader& shader)
{
    this->texture = texture.getSelf();
    this->shader = shader.getSelf();
}

void ModelObject::update(float deltaTime)
{
    GameObject::update(deltaTime);
}

void ModelObject::render()
{
    glm::mat4 modelTransform = glm::identity<glm::mat4>();
    modelTransform = glm::translate(modelTransform, position);
    modelTransform = glm::rotate(modelTransform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelTransform = glm::rotate(modelTransform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelTransform = glm::rotate(modelTransform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelTransform = glm::scale(modelTransform, size);

    shader->use();
    shader->setMat4("model", modelTransform);

    // calculate normal matrix
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelTransform)));
    shader->setMat3("normalMatrix", normalMatrix);

    // If model is set, draw it
    if (model)
    {
        model->draw(*shader);
        return;
    }

    // Else, draw a default quad
    shared_ptr<VertexArrayObject> VAO = Engine::GetDefaultVAO();
    shared_ptr<VertexBuffer> VBO = Engine::GetDefaultVBO();
    shared_ptr<IndexBuffer> IBO = Engine::GetDefaultIBO();

    shader->use();
    texture->bind();
    VAO->bind();

    glDrawArrays(GL_TRIANGLES, 0, VBO->getSize());
}

void ModelObject::onKeyDown(int key)
{

}

void ModelObject::onKeyUp(int key)
{

}