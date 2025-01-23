#pragma once
#include "GameObject.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"

class ModelObject : public GameObject
{
public:
    virtual void create(const Model& model, const Shader& shader);
    virtual void create(const Texture& texture, const Shader& shader);
    void update(float deltaTime) override;
    void render() override;
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;

protected:
    shared_ptr<Shader> shader;
    shared_ptr<Model> model;
    shared_ptr<Texture> texture;

    glm::vec3 minBounds, maxBounds;

    shared_ptr<VertexArrayObject> bboxVAO;
    shared_ptr<VertexBuffer> bboxVBO;
    shared_ptr<IndexBuffer> bboxIBO;

    void calculateBoundingBox(const glm::mat4& modelMatrix);
    void drawBoundingBox(const glm::mat4& modelMatrix);
    void prepareBoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds);
};

