#include "Model.h"
#include "Engine.h"
#include "Shader.h"

bool Model::initialize(const char* modelPath, bool gamma = false)
{
    this->path = modelPath;
    gammaCorrection = gamma;
    if (!loadModel(path))
        return false;
    return true;
}

void Model::draw(const Shader& shader)
{
    for (auto& mesh : meshes)
        mesh->draw(shader);
}

bool Model::loadModel(string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    return true;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<shared_ptr<Texture>> textures;

    VertexDefinitionElement flags = VertexDefinitionElement::POSITION;
    shared_ptr<Mesh> meshResult = make_shared<Mesh>();

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            flags |= VertexDefinitionElement::NORMAL;
        }

        // colors
        if (mesh->HasVertexColors(0))
        {
            glm::vec3 color;
            color.x = mesh->mColors[0][i].r;
            color.y = mesh->mColors[0][i].g;
            color.z = mesh->mColors[0][i].b;
            vertex.color = color;
        }
        else
        {
            vertex.color = glm::vec3(1.0f);
        }
        flags |= VertexDefinitionElement::COLOR;

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
            flags |= VertexDefinitionElement::TEXTURE_COORD;
        }

        vertices.push_back(vertex);
    }

    VertexLayoutInfo vertexLayoutInfo = VertexArrayObject::getLayoutInfo(flags);

    vector<uint8_t> vertexData( vertices.size() * vertexLayoutInfo.vertexBytes );

    for (int i = 0; i < vertexLayoutInfo.elementsCount; i++)
    {
        uint8_t* pDest = vertexData.data() + vertexLayoutInfo.elements[i].offset;
        uint8_t* pSrc = nullptr;
        switch (vertexLayoutInfo.elements[i].index)
        {
        case VertexLayoutIndex::POSITION_INDEX:
            pSrc = reinterpret_cast<uint8_t*>(&vertices[0].position);
            break;
        case VertexLayoutIndex::COLOR_INDEX:
            pSrc = reinterpret_cast<uint8_t*>(&vertices[0].color);
            break;
        case VertexLayoutIndex::TEXTURE_COORD_INDEX:
            pSrc = reinterpret_cast<uint8_t*>(&vertices[0].texCoords);
            break;
        case VertexLayoutIndex::NORMAL_INDEX:
            pSrc = reinterpret_cast<uint8_t*>(&vertices[0].normal);
            break;
        }

        for (size_t j = 0; j < vertices.size(); j++)
        {
            memcpy(pDest, pSrc, vertexLayoutInfo.elements[i].bytes);
            pDest += vertexLayoutInfo.vertexBytes;
            pSrc += sizeof(Vertex);
        }
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    vector<shared_ptr<Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    vector<shared_ptr<Texture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    meshResult->load(vertexData, vertexLayoutInfo.vertexBytes, indices, textures, flags);
    return meshResult;
}

vector<shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<shared_ptr<Texture>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::filesystem::path basePath("../Data/Textures");
        std::filesystem::path modelPath(path);
        string modelDirName = modelPath.parent_path().filename().string();

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        std::filesystem::path texturePath = basePath / modelDirName / str.C_Str();
        shared_ptr<Texture> texture = Engine::getTexture(texturePath.string().c_str());
        if (texture)
        {
            textures.push_back(texture);
            continue;
        }
    }
    return textures;
}
