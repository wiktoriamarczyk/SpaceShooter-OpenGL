#pragma once

#include "Common.h"
#include "Mesh.h"
#include "Texture.h"

class Model : public SelfHelper<Model>
{
    friend class SelfHelper<Model>;
public:
    static shared_ptr<Model> create(const char* modelPath, bool gamma = false)
    {
        return SelfHelper<Model>::create(modelPath, gamma);
    }
    void draw(const Shader& shader);
    string getPath() const { return directory; }
    vector<shared_ptr<Mesh>> getMeshes() const { return meshes; }

    glm::vec3 getMinBounds() const { return minBounds; }
    glm::vec3 getMaxBounds() const { return maxBounds; }
private:
    // model data
    vector<shared_ptr<Mesh>> meshes;
    string directory;
    string path;
    glm::vec3 minBounds, maxBounds;
    bool gammaCorrection;

    void updateBoundingBox(const vector<Vertex>& vertices);
    bool initialize(const char* modelPath, bool gamma);
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    bool loadModel(string const& path);
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    vector<shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

