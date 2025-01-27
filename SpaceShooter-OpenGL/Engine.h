#pragma once

#include "Common.h"
#include "GameObject.h"
#include "GameState.h"

class VertexBuffer;
class IndexBuffer;
class Shader;
class VertexArrayObject;
class Model;
class Shader;
class Player;
class EnemyUnit;
class Projectile;
class ModelObject;
class Asteroid;
class AsteroidSpawner;
class EnemySpawner;
class PointLight;
class HealthBar;

class Engine
{
public:
    static bool init();
    static void run();

    void addGameObject(shared_ptr<GameObject> gameObject);
    void addPointLight(shared_ptr<PointLight> pointLight);
    void removePointLight(PointLight& pointLight);
    void setPlayerHealth(float newHealth);
    glm::vec3 getPlayerPosition() const;
    glm::vec3 getCameraPosition() const { return cameraPosition; };
    void renderText(string text, glm::vec2 position, float scale=0.5f, glm::vec3 color = WHITE_COLOR);
    void changeGameState(GAME_STATE newState);
    void exitGame();
    void playSound(const char* path);
    vector<shared_ptr<Texture>> loadStarsTextures();
    vector<shared_ptr<Model>> loadAsteroidModels();
    glm::vec3 getMouseWorldRayDirection()const;
    glm::vec3 getMouseWorldPosOnTestPlane(float TestPlaneZ=-7.f)const;
    void toggleWireframeMode() { wireframeMode = !wireframeMode; }

    static Engine& getInstance() { return instance; }

    static shared_ptr<Texture> getTexture(const char* path, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE);
    static shared_ptr<Model> getModel(const char* path);
    static shared_ptr<Shader> getShader(const char* vertexPath, const char* fragmentPath);

    static shared_ptr<VertexArrayObject> GetDefaultVAO();
    static shared_ptr<VertexBuffer> GetDefaultVBO();
    static shared_ptr<IndexBuffer> GetDefaultIBO();

    static shared_ptr<Shader> GetDefaultSpriteShader();
    static shared_ptr<Shader> GetDefaultModelShader();
    static shared_ptr<Shader> GetDefaultEmissiveShader();
    static shared_ptr<Shader> GetDefaultBBoxShader();

    static glm::vec2 getMousePosition();
    static glm::vec2 getMousePositionInUISpace();
    static glm::vec2 getScreenSize();

private:
    Engine() = default;
    static Engine instance;

    GameState* currentState = nullptr;
    vector<unique_ptr<GameState>>  allStates;

    shared_ptr<VertexBuffer> defaultVBO;
    shared_ptr<IndexBuffer> defaultIBO;
    shared_ptr<VertexArrayObject> defaultVAO;
    shared_ptr<VertexBuffer> textVBO;
    shared_ptr<VertexArrayObject> textVAO;

    shared_ptr<Shader> defaultSpriteShader;
    shared_ptr<Shader> defaultModelShader;
    shared_ptr<Shader> defaultEmissiveShader;
    shared_ptr<Shader> defaultBBoxShader;
    shared_ptr<Shader> defaultTextShader;
    shared_ptr<Shader> postprocessShader;

    GLFWwindow* window = nullptr;
    sf::Music music;
    unordered_map<string, sf::SoundBuffer> soundBuffers;
    vector<shared_ptr<sf::Sound>> activeSounds;
    shared_ptr<Sprite> background;
    vector<shared_ptr<Texture>> textures;
    vector<shared_ptr<Model>> models;
    vector<shared_ptr<Shader>> shaders;
    vector<weak_ptr<PointLight>> pointLights;
    map<char, Character> characters;

    double lastFrame = 0;
    bool initializing = true;
    bool wireframeMode = false;
    const glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 projectionMatrix;
    unsigned int renderTextureframebufferID = 0;
    unsigned int renderTextureDepthBufferID = 0;
    unsigned int windowWidth = SCREEN_WIDTH;
    unsigned int windowHeight = SCREEN_HEIGHT;
    shared_ptr<Texture> renderTexture;
    shared_ptr<Sprite> fullscreenQuad;

    static void processInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void processMouseInput(GLFWwindow* window, int button, int action, int mods);
    static void onWindowSizeChanged(GLFWwindow* window, int width, int height);
    void processInput(int key, int scancode, int action, int mods);
    void processMouseInput(int button, int action, int mods);
    void onWindowSizeChanged(int width, int height);
    void deleteFramebuffer();
    void recreateFramebuffer();
    bool doInit();
    void doRun();
    void render();
    void update(float deltaTime);
    bool freeTypeInit();
    bool createDefaultResources();
    void setCustomCursor();
};

