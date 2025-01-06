#include "Mesh.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

bool Mesh::load(vector<uint8_t> verticesData, vector<unsigned int> indices, vector<shared_ptr<Texture>> textures, VertexDefinitionElement flags)
{
    if (verticesData.empty())
    {
        std::cout << "Mesh::create: vertices empty" << std::endl;
        return false;
    }

    this->verticesData = verticesData;
    this->indices = indices;
    this->textures = move(textures);
    this->flags = flags;

    setupMesh();

    return true;
}

void Mesh::setupMesh()
{
    VAO = make_shared<VertexArrayObject>();
    VBO = make_shared<VertexBuffer>();
    EBO = make_shared<IndexBuffer>();

    VBO->create(verticesData.data(), verticesData.size());
    EBO->create(indices.data(), indices.size()*sizeof(indices[0]));

    VAO->create(*VBO, this->flags );
}

void Mesh::draw(const Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i]->bind(i);
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name;

        if (textures[i]->getType() == aiTextureType::aiTextureType_DIFFUSE)
        {
            number = to_string(diffuseNr++);
            name = "texture_diffuse";
        }

        else if (textures[i]->getType() == aiTextureType::aiTextureType_SPECULAR)
        {
            number = to_string(specularNr++);
            name = "texture_specular";
        }

        shader.setInt((name + number).c_str(), i);
    }

    VAO->bind();
    EBO->bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO->unbind();
}

