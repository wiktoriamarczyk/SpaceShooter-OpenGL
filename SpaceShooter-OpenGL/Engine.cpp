#include "Engine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "TestCube.h"
#include "Model.h"
#include "ModelObject.h"
#include "Player.h"
#include "EnemyUnit.h"
#include "Projectile.h"
#include "AsteroidSpawner.h"
#include "StarSpawner.h"
#include "EnemySpawner.h"
#include "Billboard.h"

Engine Engine::instance;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Engine::processInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    instance.processInput(key, scancode, action, mods);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Engine::processInput(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        for (auto& gameObject : gameObjects)
        {
            gameObject->onKeyDown(key);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        for (auto& gameObject : gameObjects)
        {
            gameObject->onKeyUp(key);
        }
        // If the escape key is pressed, close the window
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    }
}

void Engine::processMouseInput(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        // move light to the position of the cursor
        //double xpos, ypos;
        //glfwGetCursorPos(window, &xpos, &ypos);
        //int width, height;
        //glfwGetWindowSize(window, &width, &height);
        //glm::vec3 lightPos = glm::vec3((xpos / width) * 2 - 1, 1 - (ypos / height) * 2, 0.0f);
        //instance.lightCube->setPosition(lightPos);
    }
    else if (action == GLFW_RELEASE)
    {

    }

}

bool Engine::init()
{
    return instance.doInit();
}

bool Engine::doInit()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    glfwSetFramebufferSizeCallback(window, [] (GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // glfw: set the callback function for key press
    glfwSetKeyCallback(window, processInputCallback);
    glfwSetMouseButtonCallback(window, processMouseInput);

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);

    if (WIREFRAME_MODE)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Create default object
    if (!createDefaultResources())
    {
        std::cout << "Failed to create default resources" << std::endl;
        return false;
    }

    createGameObjects();

    return true;
}

bool Engine::createDefaultResources()
{
    // Create quad for sprite rendering
    float vertices[] = {
        // positions          // texture coords
        0.5f,   0.5f, 0.0f,   1.0f, 1.0f,        // top right
        0.5f,  -0.5f, 0.0f,   1.0f, 0.0f,        // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,        // bottom left

        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,        // top left
        0.5f,   0.5f, 0.0f,   1.0f, 1.0f,        // top right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,        // bottom left
    };

    defaultVBO = make_shared<VertexBuffer>();
    if (!defaultVBO->create(vertices, sizeof(vertices)))
        return false;

    defaultIBO = make_shared<IndexBuffer>();

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    if (!defaultIBO->create(indices, sizeof(indices)))
        return false;

    defaultVAO = make_shared<VertexArrayObject>();
    defaultVAO->create(*defaultVBO, VertexDefinitionElement::POSITION | VertexDefinitionElement::TEXTURE_COORD);


    // Init default sprite resources
    defaultSpriteShader = Shader::create("../Data/Shaders/sprite_shader.vs", "../Data/shaders/sprite_shader.fs");
    if (!defaultSpriteShader)
        return false;
    defaultSpriteShader->use();

    // set orthographic projection matrix
    auto ortho = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    defaultSpriteShader->setMat4("projection", ortho);
    defaultSpriteShader->setMat4("view", glm::identity<glm::mat4>());


    // Init default model resources
    defaultModelShader = Shader::create("../Data/Shaders/shader.vs", "../Data/shaders/shader.fs");
    if (!defaultModelShader)
        return false;
    defaultModelShader->use();

    // set perspective projection matrix
    auto projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    defaultModelShader->setMat4("projection", projection);
    defaultModelShader->setMat4("view",glm::identity<glm::mat4x4>());
    defaultModelShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    defaultModelShader->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f));

    // set material properties
    defaultModelShader->setVec3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    defaultModelShader->setVec3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    defaultModelShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    defaultModelShader->setFloat("material.shininess", 32.0f);

    // set light properties
    defaultModelShader->setVec3("dirLight.direction", glm::vec3(0.f, -1.0f, -2.f));
    defaultModelShader->setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    defaultModelShader->setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    defaultModelShader->setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));


    // Init default lighting shader
    defaultLightingShader = Shader::create("../Data/Shaders/light_source_shader.vs", "../Data/shaders/light_source_shader.fs");
    if (!defaultLightingShader)
        return false;
    defaultLightingShader->use();
    defaultLightingShader->setMat4("projection", projection);
    defaultLightingShader->setMat4("view", glm::identity<glm::mat4x4>());
}

void Engine::createGameObjects()
{
    player = make_shared<Player>();

    shared_ptr<Model> playerModel = getModel(PLAYER_MODEL_PATH);
    shared_ptr<Model> modelProjectile = getModel(PROJECTILE_MODEL_PATH);

    if (playerModel && modelProjectile)
    {
        player->create(*playerModel, *defaultModelShader, *modelProjectile);
        player->setSize(glm::vec3(0.1f, 0.1f, 0.1f));
        player->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
        gameObjects.push_back(player);
    }

    shared_ptr<AsteroidSpawner> asteroidSpawner = make_shared<AsteroidSpawner>();

    const char* asteroidModelPath1 = "../Data/Models/Asteroids/asteroid/scene.gltf";
    const char* asteroidModelPath2 = "../Data/Models/Asteroids/asteroid_01/scene.gltf";
    const char* asteroidModelPath3 = "../Data/Models/Asteroids/asteroid-1a-game-model/source/Asteroid_1a.glb";
    //const char* asteroidModelPath4 = "../Data/Models/Asteroids/metal_asteroid/scene.gltf";

    vector<shared_ptr<Model>> asteroidModels;
    asteroidModels.push_back(getModel(asteroidModelPath1));
    asteroidModels.push_back(getModel(asteroidModelPath2));
    asteroidModels.push_back(getModel(asteroidModelPath3));
    //asteroidModels.push_back(getModel(asteroidModelPath4));

    if (!asteroidModels.empty())
    {
        asteroidSpawner->create(asteroidModels, *defaultModelShader);
        gameObjects.push_back(asteroidSpawner);
    }

    shared_ptr<StarSpawner> starSpawner = make_shared<StarSpawner>();
    vector<shared_ptr<Texture>> starTextures = loadStarsTextures();
    if (!starTextures.empty())
    {
        starSpawner->create(starTextures, *defaultSpriteShader);
        gameObjects.push_back(starSpawner);
    }

    shared_ptr<EnemySpawner> enemySpawner = make_shared<EnemySpawner>();
    vector<shared_ptr<Model>> enemyModels;
    enemyModels.push_back(getModel(ENEMY_MODEL_PATH));

    if (!enemyModels.empty() && modelProjectile)
    {
        enemySpawner->create(enemyModels, *defaultModelShader, *modelProjectile);
        gameObjects.push_back(enemySpawner);
    }
}

void Engine::addGameObject(shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
}

glm::vec3 Engine::getPlayerPosition() const {
    if (player)
    {
        return player->getPosition();
    }
    return glm::vec3(0.0f);
}

shared_ptr<Texture> Engine::getTexture(const char* path, aiTextureType type)
{
    auto foundTexture = std::find_if(instance.textures.begin(), instance.textures.end(), [path](const shared_ptr<Texture>& texture)
        {
            return texture->getPath() == path;
        });

    if (foundTexture != instance.textures.end())
        return *foundTexture;

    auto texture = Texture::create(path, type);
    if (!texture)
        return nullptr;

    instance.textures.push_back(texture);
    return texture;
}

shared_ptr<Model> Engine::getModel(const char* path)
{
    auto foundModel = std::find_if(instance.models.begin(), instance.models.end(), [path](const shared_ptr<Model>& model)
        {
            return model->getPath() == path;
        });

    if (foundModel != instance.models.end())
        return *foundModel;

    auto model = Model::create(path);
    if (!model)
        return nullptr;

    instance.models.push_back(model);
    return model;
}

shared_ptr<Shader> Engine::getShader(const char* vertexPath, const char* fragmentPath)
{
    auto foundShader = std::find_if(instance.shaders.begin(), instance.shaders.end(), [vertexPath, fragmentPath](const shared_ptr<Shader>& shader)
        {
            return shader->getVertexPath() == vertexPath && shader->getFragmentPath() == fragmentPath;
        });

    if (foundShader != instance.shaders.end())
        return *foundShader;

    auto shader = Shader::create(vertexPath, fragmentPath);
    if (!shader)
        return nullptr;

    instance.shaders.push_back(shader);
    return shader;
}

void Engine::update(float deltaTime)
{
    for (int i = 0; i < gameObjects.size();)
    {
        if (gameObjects[i]->isAlive())
            gameObjects[i++]->update(deltaTime);
        else
            gameObjects.erase(gameObjects.begin() + i);
    }
}

void Engine::render()
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer and depth buffer

    for (int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->render();
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
}

void Engine::run()
{
    instance.doRun();
}

void Engine::doRun()
{
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        double currentFrame = glfwGetTime();
        double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        update(deltaTime);
        render();
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

vector<shared_ptr<Texture>> Engine::loadStarsTextures()
{
    vector<shared_ptr<Texture>> textures;
    const char* path1 = "../Data/Textures/star1.png";
    const char* path2 = "../Data/Textures/star2.png";
    const char* path3 = "../Data/Textures/star3.png";
    const char* path4 = "../Data/Textures/star4.png";
    const char* path5 = "../Data/Textures/star5.png";
    const char* path6 = "../Data/Textures/star6.png";
    const char* path7 = "../Data/Textures/star7.png";

    textures.push_back(getTexture(path1));
    textures.push_back(getTexture(path2));
    textures.push_back(getTexture(path3));
    textures.push_back(getTexture(path4));
    textures.push_back(getTexture(path5));
    textures.push_back(getTexture(path6));
    textures.push_back(getTexture(path7));

    return textures;
}

shared_ptr<VertexBuffer> Engine::GetDefaultVBO()
{
    return instance.defaultVBO;
}

shared_ptr<IndexBuffer> Engine::GetDefaultIBO()
{
    return instance.defaultIBO;
}

shared_ptr<VertexArrayObject> Engine::GetDefaultVAO()
{
    return instance.defaultVAO;
}

shared_ptr<Shader> Engine::GetDefaultSpriteShader()
{
    return instance.defaultSpriteShader;
}

shared_ptr<Shader> Engine::GetDefaultModelShader()
{
    return instance.defaultModelShader;
}