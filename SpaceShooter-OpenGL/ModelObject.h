#pragma once
#include "GameObject.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "Plane.h"

class ModelObject : public GameObject
{
public:
    virtual void create(const Model& model, const Shader& shader);
    virtual void create(const Texture& texture, const Shader& shader);
    void update(float deltaTime) override;
    void render() override;
    void onKeyDown(int key) override {};
    void onKeyUp(int key) override {};
    vector<glm::vec3> getBboxVertices() const { return bboxVertices; }
    vector<Plane> getBboxPlanes() const { return bboxPlanes; }
    glm::vec3 getWorldBboxCenter() const { return glm::vec3(modelMatrix * glm::vec4(bboxCenter, 1.0f)); }
    bool isVertexInsideBbox(glm::vec3 vertex);
    void updateHealth(float value) override;

protected:
    shared_ptr<Shader> shader;
    shared_ptr<Model> model;
    shared_ptr<Texture> texture;

    shared_ptr<VertexArrayObject> bboxVAO;
    shared_ptr<VertexBuffer> bboxVBO;
    shared_ptr<IndexBuffer> bboxIBO;

    vector<glm::vec3> bboxVertices;
    glm::vec3 bboxCenter;
    glm::vec3 color = WHITE_COLOR;
    glm::vec3 hitColor = glm::vec3(0.8, 0.8, 0.8);
    float hitAnimation = 0;
    vector<Plane> bboxPlanes;

    glm::mat4 modelMatrix;
    glm::mat4 inverseModelMatrix;

    void drawBoundingBox(const glm::mat4& modelMatrix);
    void prepareBoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds);
};

