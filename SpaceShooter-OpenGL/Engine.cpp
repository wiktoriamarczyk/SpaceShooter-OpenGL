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
        for (auto& gameObject : instance.gameObjects)
        {
            gameObject->onKeyDown(key);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        for (auto& gameObject : instance.gameObjects)
        {
            gameObject->onKeyUp(key);
        }
        // If the escape key is pressed, close the window
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
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

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);

    if (WIREFRAME_MODE)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }


    // Create default object
    if (!createDefaultResources())
        return false;

    createGameObjects();

    return true;
}

bool Engine::createDefaultResources()
{
    defaultVBO = make_shared<VertexBuffer>();

    float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

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
    // set identity matrix for view
    defaultSpriteShader->setMat4("view", glm::identity<glm::mat4>());

    // Init default model resources
    defaultModelShader = Shader::create("../Data/Shaders/shader.vs", "../Data/shaders/shader.fs");
    if (!defaultModelShader)
        return false;
    defaultModelShader->use();

    // set perspective projection matrix
    auto projection = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    defaultModelShader->setMat4("projection", projection);
    // set view matrix
    defaultModelShader->setMat4("view",glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(0.0f, 0.0f, -2.0f)));
}

void Engine::createGameObjects()
{
    // TODO: Create game objects here
    //shared_ptr<TestCube> testCube = make_shared<TestCube>();
    //gameObjects.push_back(testCube);

    // Create player
    player = make_shared<Player>();
    shared_ptr<Model> model = getModel(PLAYER_MODEL_PATH);
    if (model)
    {
        player->create(*model, *defaultModelShader);
        player->setSize(glm::vec3(0.1f, 0.1f, 0.1f));
        gameObjects.push_back(player);
    }

    enemy = make_shared<EnemyUnit>();
    projectile = make_shared<Projectile>();
    shared_ptr<Model> modelEnemy = getModel(ENEMY_MODEL_PATH);
    shared_ptr<Model> modelProjectile = getModel(PROJECTILE_MODEL_PATH);
    if (modelEnemy)
    {
        enemy->create(*modelEnemy, *defaultModelShader, *modelProjectile);
        enemy->setSize(glm::vec3(0.05f, 0.05f, 0.05f));
        gameObjects.push_back(enemy);
    }

    //if (modelProjectile)
    //{
    //    projectile->create(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, *modelProjectile, *defaultModelShader);
    //    projectile->setSize(glm::vec3(0.05f, 0.05f, 0.05f));
    //    gameObjects.push_back(projectile);
    //}

}

void Engine::addGameObject(shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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