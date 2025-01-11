#pragma once

#include "Common.h"
#include "GameObject.h"

class VertexBuffer;
class IndexBuffer;
class Shader;
class VertexArrayObject;
class Model;
class Shader;
class Player;
class EnemyUnit;
class Projectile;

class Engine
{
public:
    static bool init();
    static void run();

    static shared_ptr<Texture> getTexture(const char* path, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE);
    static shared_ptr<Model> getModel(const char* path);
    static shared_ptr<Shader> getShader(const char* vertexPath, const char* fragmentPath);

    static shared_ptr<VertexBuffer> GetDefaultVBO();
    static shared_ptr<IndexBuffer> GetDefaultIBO();
    static shared_ptr<Shader> GetDefaultSpriteShader();
    static shared_ptr<Shader> GetDefaultModelShader();
    static shared_ptr<VertexArrayObject> GetDefaultVAO();

private:
    Engine() = default;
    static Engine instance;

    shared_ptr<VertexBuffer> defaultVBO;
    shared_ptr<IndexBuffer> defaultIBO;
    shared_ptr<VertexArrayObject> defaultVAO;
    shared_ptr<Shader> defaultSpriteShader;
    shared_ptr<Shader> defaultModelShader;

    GLFWwindow* window = nullptr;
    shared_ptr<Player> player;
    shared_ptr<EnemyUnit> enemy;
    shared_ptr<Projectile> projectile;
    vector<shared_ptr<GameObject>> gameObjects;
    vector<shared_ptr<Texture>> textures;
    vector<shared_ptr<Model>> models;
    vector<shared_ptr<Shader>> shaders;

    double lastFrame = 0;

    static void processInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void processInput(int key, int scancode, int action, int mods);
    void render();
    void update(float deltaTime);
    void createGameObjects();
    bool createDefaultResources();
    bool doInit();
    void doRun();

};

