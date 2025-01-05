#pragma once

#include "Common.h"
#include "GameObject.h"

class VertexBuffer;
class IndexBuffer;
class Shader;
class VertexArrayObject;

class Engine
{
public:
    bool init();
    void run();

    static shared_ptr<Texture> getTexture(const char* path);

    static shared_ptr<VertexBuffer> GetDefaultVBO();
    static shared_ptr<IndexBuffer> GetDefaultIBO();
    static shared_ptr<Shader> GetDefaultShader();
    static shared_ptr<VertexArrayObject> GetDefaultVAO();

private:
    static Engine instance;

    shared_ptr<VertexBuffer> defaultVBO;
    shared_ptr<IndexBuffer> defaultIBO;
    shared_ptr<VertexArrayObject> defaultVAO;
    shared_ptr<Shader> defaultShader;

    GLFWwindow* window = nullptr;
    vector<shared_ptr<GameObject>> gameObjects;
    vector<shared_ptr<Texture>> textures;

    double lastFrame = 0;

    static void processInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void processInput(int key, int scancode, int action, int mods);
    void render();
    void update(float deltaTime);
    void createGameObjects();
};

