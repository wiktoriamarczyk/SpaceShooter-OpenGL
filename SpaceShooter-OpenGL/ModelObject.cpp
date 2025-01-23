#include "ModelObject.h"
#include "Model.h"
#include "Shader.h"
#include "Engine.h"
#include "Common.h"
#include "IndexBuffer.h"

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

    glDrawArrays(GL_TRIANGLES, 0, VBO->getCount());
}

void ModelObject::onKeyDown(int key)
{

}

void ModelObject::onKeyUp(int key)
{

}

void ModelObject::calculateBoundingBox(const glm::mat4 &modelMatrix)
{
    if (!model)
        return;

    minBounds = glm::vec3(FLT_MAX);
    maxBounds = glm::vec3(-FLT_MAX);

    for (const auto& mesh : model->getMeshes())
    {
        const auto& vertices = mesh->getVertices();
        for (const auto& vertex : vertices)
        {
            glm::vec3 pos = vertex.position;

            minBounds.x = std::min(minBounds.x, pos.x);
            minBounds.y = std::min(minBounds.y, pos.y);
            minBounds.z = std::min(minBounds.z, pos.z);

            maxBounds.x = std::max(maxBounds.x, pos.x);
            maxBounds.y = std::max(maxBounds.y, pos.y);
            maxBounds.z = std::max(maxBounds.z, pos.z);
        }
    }

    minBounds = glm::vec3(modelMatrix * glm::vec4(minBounds, 1.0f));
    maxBounds = glm::vec3(modelMatrix * glm::vec4(maxBounds, 1.0f));
}

void ModelObject::drawBoundingBox(const glm::mat4& modelMatrix)
{
    // Use the default bounding box shader
    auto shader = Engine::GetDefaultBBoxShader();
    shader->use();
    shader->setMat4("model", modelMatrix);

    bboxVAO->bind();
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}

void ModelObject::prepareBoundingBox(const glm::vec3& min, const glm::vec3& max)
{
    // Define the 8 vertices of the bounding box
    float vertices[72] = {
        min.x, min.y, min.z,  max.x, min.y, min.z,
        max.x, min.y, min.z,  max.x, max.y, min.z,
        max.x, max.y, min.z,  min.x, max.y, min.z,
        min.x, max.y, min.z,  min.x, min.y, min.z,
        min.x, min.y, max.z,  max.x, min.y, max.z,
        max.x, min.y, max.z,  max.x, max.y, max.z,
        max.x, max.y, max.z,  min.x, max.y, max.z,
        min.x, max.y, max.z,  min.x, min.y, max.z,
        min.x, min.y, min.z,  min.x, min.y, max.z,
        max.x, min.y, min.z,  max.x, min.y, max.z,
        max.x, max.y, min.z,  max.x, max.y, max.z,
        min.x, max.y, min.z,  min.x, max.y, max.z,
    };

    unsigned int indices[24] = {
        0, 1,  1, 2,  2, 3,  3, 0,  // Bottom face
        4, 5,  5, 6,  6, 7,  7, 4,  // Top face
        0, 4,  1, 5,  2, 6,  3, 7   // Vertical lines
    };

    // Create VAO/VBO
    bboxVBO = make_shared<VertexBuffer>();
    bboxVBO->create(vertices, 3, sizeof(vertices) / (3 * sizeof(float)));

    bboxIBO = make_shared<IndexBuffer>();
    bboxIBO->create(indices, true, sizeof(indices) / sizeof(unsigned int));

    bboxVAO = make_shared<VertexArrayObject>();
    bboxVAO->create(*bboxVBO, VertexDefinitionElement::POSITION);
}
