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
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;
    vector<glm::vec3> getBoundingBoxVertices() const { return bboxVertices; }
    vector<Plane> getBoundingBoxPlanes() const { return bboxPlanes; }
    bool isBboxIntersectingBbox(const ModelObject& other);

    static bool isBboxIntersectingPlane(const Plane& plane, const std::vector<glm::vec3>& vertices);

protected:
    shared_ptr<Shader> shader;
    shared_ptr<Model> model;
    shared_ptr<Texture> texture;

    shared_ptr<VertexArrayObject> bboxVAO;
    shared_ptr<VertexBuffer> bboxVBO;
    shared_ptr<IndexBuffer> bboxIBO;

    vector<glm::vec3> bboxVertices;
    vector<Plane> bboxPlanes;

    void drawBoundingBox(const glm::mat4& modelMatrix);
    void prepareBoundingBox(const glm::vec3& minBounds, const glm::vec3& maxBounds);
};

