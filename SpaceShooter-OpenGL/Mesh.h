#pragma once

#include "Common.h"
#include "VertexArrayObject.h"

class Texture;
class VertexBuffer;
class IndexBuffer;
class Shader;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    Mesh() = default;
    bool load(vector<uint8_t> verticesData, vector<unsigned int> indices, vector<shared_ptr<Texture>> textures, VertexDefinitionElement flags);
    void draw(const Shader& shader);
private:
    //  render data
    shared_ptr<VertexArrayObject> VAO;
    shared_ptr<VertexBuffer> VBO;
    shared_ptr<IndexBuffer> EBO;

    // mesh data
    vector<uint8_t> verticesData;
    vector<unsigned int> indices;
    vector<shared_ptr<Texture>> textures;
    VertexDefinitionElement flags;

    void setupMesh();
};

