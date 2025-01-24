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

    prepareBoundingBox(model.getMinBounds(), model.getMaxBounds());
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
    shader->setFloat("appTime", (float)glfwGetTime());

    // If model is set, draw it
    if (model)
    {
        model->draw(*shader);


        if (bboxVBO != nullptr)
            drawBoundingBox(modelTransform);
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

void ModelObject::drawBoundingBox(const glm::mat4& modelMatrix)
{
    // Use the default bounding box shader
    auto shader = Engine::GetDefaultBBoxShader();
    shader->use();
    shader->setMat4("model", modelMatrix);

    bboxVAO->bind();
    bboxIBO->bind();

    //draw lines instead of triangles
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}

void ModelObject::prepareBoundingBox(const glm::vec3& min, const glm::vec3& max)
{
    // Define the 8 vertices of the bounding box
    float vertices[] = {
        min.x, min.y, min.z,
        max.x, min.y, min.z,
        max.x, max.y, min.z,
        min.x, max.y, min.z,

        min.x, min.y, max.z,
        max.x, min.y, max.z,
        max.x, max.y, max.z,
        min.x, max.y, max.z,
    };

    uint32_t indices[] = {
        0, 1,  1, 2,  2, 3,  3, 0,  // Bottom face
        4, 5,  5, 6,  6, 7,  7, 4,  // Top face
        0, 4,  1, 5,  2, 6,  3, 7   // Vertical lines
    };

    // Create VAO/VBO
    bboxVBO = make_shared<VertexBuffer>();
    bboxVBO->create(vertices, sizeof(float) * 3, sizeof(vertices) / (3 * sizeof(float)));

    bboxIBO = make_shared<IndexBuffer>();
    bboxIBO->create(indices, true, sizeof(indices) / sizeof(unsigned int));

    bboxVAO = make_shared<VertexArrayObject>();
    bboxVAO->create(*bboxVBO, VertexDefinitionElement::POSITION);
}
