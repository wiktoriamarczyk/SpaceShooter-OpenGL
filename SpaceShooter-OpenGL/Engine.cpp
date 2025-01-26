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
#include "ChargingStation.h"
#include "Button.h"

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
        for (int i=0; i<gameObjects.size(); i++)
        {
            gameObjects[i]->onKeyDown(key);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        for (int i=0; i<gameObjects.size(); i++)
        {
            gameObjects[i]->onKeyUp(key);
        }
        // If the escape key is pressed, close the window
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    }
}

void Engine::processMouseInput(GLFWwindow* window, int button, int action, int mods)
{
    instance.processMouseInput(button, action, mods);
}

void Engine::processMouseInput(int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        for (int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->onMouseButtonDown(button);
            this->button->onMouseButtonDown(button);
        }
    }
    else if (action == GLFW_RELEASE)
    {
        for (int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->onMouseButtonUp(button);
            this->button->onMouseButtonUp(button);
        }
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

    setCustomCursor();
    createGameObjects();

    return true;
}

void Engine::setCustomCursor()
{
    int width, height, channels;
    unsigned char* cursorImage = loadImage(CURSOR_PATH, &width, &height, &channels);
    if (!cursorImage)
    {
        std::cout << "Failed to load cursor image" << std::endl;
        return;
    }

    GLFWimage image;
    image.width = width;
    image.height = height;
    image.pixels = cursorImage;

    GLFWcursor* customCursor = glfwCreateCursor(&image, width / 2, height / 2); // width/2, height/2 to hotspot
    if (!customCursor)
    {
        std::cerr << "Failed to create custom cursor!" << std::endl;
        stbi_image_free(cursorImage);
        return;
    }

    glfwSetCursor(window, customCursor);
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
    if (!defaultVBO->create(vertices, 20, sizeof(vertices)/20))
        return false;

    defaultIBO = make_shared<IndexBuffer>();

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    if (!defaultIBO->create(indices, true, sizeof(indices)/4))
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
    defaultModelShader->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f));
    defaultModelShader->setVec3("color", WHITE_COLOR);
    defaultModelShader->setVec3("ambient", glm::vec3(0.15f, 0.15f, 0.15f));

    // set material properties
    defaultModelShader->setVec3("material.ambient", WHITE_COLOR);
    defaultModelShader->setVec3("material.diffuse", WHITE_COLOR);
    defaultModelShader->setVec3("material.specular", WHITE_COLOR);
    defaultModelShader->setFloat("material.shininess", 64.0f);

    // set light properties
    defaultModelShader->setVec3("dirLight.direction", glm::vec3(0.f, -1.0f, 0.f));
    defaultModelShader->setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
    defaultModelShader->setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));


    // Init default lighting shader
    defaultEmissiveShader = Shader::create(VS_EMISSIVE_FILE_NAME.c_str(), FS_EMISSIVE_FILE_NAME.c_str());
    if (!defaultEmissiveShader)
        return false;
    defaultEmissiveShader->use();
    defaultEmissiveShader->setMat4("projection", projection);
    defaultEmissiveShader->setMat4("view", glm::identity<glm::mat4x4>());


    // Init default bounding box shader
    defaultBBoxShader = Shader::create(VS_BBOX_FILE_NAME.c_str(), FS_BBOX_FILE_NAME.c_str());
    if (!defaultBBoxShader)
        return false;
    defaultBBoxShader->use();
    defaultBBoxShader->setMat4("projection", projection);
    defaultBBoxShader->setMat4("view", glm::identity<glm::mat4x4>());
    defaultBBoxShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
}

void Engine::createGameObjects()
{
    // Draw background sprite
    shared_ptr<Texture> backgroundTexture = getTexture("../Data/Textures/background.png");
    background = Sprite::create(*backgroundTexture, glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    auto player = make_shared<Player>();
    this->player = player;

    shared_ptr<Model> playerModel = getModel(PLAYER_MODEL_PATH);
    shared_ptr<Model> modelProjectile = getModel(PROJECTILE_MODEL_PATH);
    if (playerModel && modelProjectile)
    {
        player->create(*playerModel, *defaultModelShader, *modelProjectile);
        player->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
        gameObjects.push_back(player);
    }

    shared_ptr<AsteroidSpawner> asteroidSpawner = make_shared<AsteroidSpawner>();
    vector<shared_ptr<Model>> asteroidModels = loadAsteroidModels();
    if (!asteroidModels.empty())
    {
        asteroidSpawner->create(asteroidModels, *defaultModelShader);
        gameObjects.push_back(asteroidSpawner);
    }

    shared_ptr<StarSpawner> starSpawner = make_shared<StarSpawner>();
    vector<shared_ptr<Texture>> starTextures = loadStarsTextures();
    if (!starTextures.empty())
    {
        starSpawner->create(starTextures, *defaultEmissiveShader);
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

    shared_ptr<ChargingStation> station = make_shared<ChargingStation>();
    vector<shared_ptr<Model>> stationModels;
    stationModels.push_back(getModel(STATION_MODEL_PATH));
    if (!stationModels.empty())
    {
        shared_ptr<Model> selectedModel = stationModels[0];
        station->create(*selectedModel, *defaultModelShader);
        gameObjects.push_back(station);
    }

    button = make_shared<Button>();
    button->create(glm::vec2(200,50), glm::vec2(200.0f, 50.0f), "Play", [player=this->player]()
        {
            if (auto p = player.lock())
                p->updateHealth(-10.f);
        });
}

void Engine::addGameObject(shared_ptr<GameObject> gameObject)
{
    gameObjects.push_back(gameObject);
}

void Engine::addPointLight(shared_ptr<PointLight> pointLight)
{
    assert(pointLight->getIndex() == -1);

    int newIndex = pointLights.size();
    pointLight->create(newIndex);

    pointLights.push_back(pointLight);

    auto modelShader = GetDefaultModelShader();
    modelShader->setInt("pointLightsCount", pointLights.size());
}

void Engine::removePointLight(PointLight& pointLight)
{
    int shaderIndex = pointLight.getIndex();

    // TODO: Fix error when closing the app
    if (pointLights.size() > 0)
        pointLights.erase(pointLights.begin() + shaderIndex);

    // Update shader lights count in model shader
    auto modelShader = pointLight.getModelShader();
    modelShader->use();
    modelShader->setInt("pointLightsCount", pointLights.size());

    // Move indices of objects that are rendered after the deleted object
    for (int i = shaderIndex; i < pointLights.size(); ++i)
    {
        auto light = pointLights[i].lock();
        if (light)
        {
            light->setNewIndex(i);
        }
    }
}

glm::vec3 Engine::getPlayerPosition() const
{
    if (auto player = this->player.lock())
    {
        return player->getPosition();
    }
    return glm::vec3(0.0f);
}

void Engine::setPlayerHealth(float newHealth) {
    auto player = this->player.lock(); // Sprawdza, czy gracz istnieje
    if (!player)
        return;
    player->updateHealth(newHealth);
}

void Engine::update(float deltaTime)
{
    // Update game objects
    for (int i = 0; i < gameObjects.size();)
    {
        if (gameObjects[i]->isAlive())
            gameObjects[i++]->update(deltaTime);
        else
            gameObjects.erase(gameObjects.begin() + i);
    }

    button->update(deltaTime);

    // Get all projectiles
    vector<shared_ptr<Projectile>> enemyProjectiles;
    vector<shared_ptr<Projectile>> playerProjectiles;
    vector<shared_ptr<EnemyUnit>> enemies;

    for (int i = 0; i < gameObjects.size(); ++i)
    {
        auto projectile = dynamic_pointer_cast<Projectile>(gameObjects[i]);
        if (projectile)
        {
            if (projectile->getTeam() == TEAM::ENEMY)
                enemyProjectiles.push_back(projectile);
            else
                playerProjectiles.push_back(projectile);

            continue;
        }
        auto enemy = dynamic_pointer_cast<EnemyUnit>(gameObjects[i]);
        if (enemy)
        {
            enemies.push_back(enemy);
        }
    }

    // Check for collisions with projectiles
    for (const auto& projectile : playerProjectiles)
    {
        for (const auto& enemy : enemies)
        {
            if (enemy->isVertexInsideBbox(projectile->getWorldBboxCenter()))
            {
                enemy->updateHealth(-PROJECTILE_DAMAGE);
                projectile->setAlive(false);
            }
        }
    }

    auto player = this->player.lock();
    if (!player)
        return;

    for (const auto& projectile : enemyProjectiles)
    {
        if (player->isVertexInsideBbox(projectile->getWorldBboxCenter()))
        {
            player->updateHealth(-PROJECTILE_DAMAGE);
            projectile->setAlive(false);
        }
    }
}

void Engine::render()
{
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    background->draw();
    // clear the depth buffer after drawing the background
    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->render();
    }

    button->render();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
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

vector<shared_ptr<Model>> Engine::loadAsteroidModels()
{
    vector<shared_ptr<Model>> asteroidModels;
    const char* asteroidModelPath1 = "../Data/Models/Asteroids/asteroid/scene.gltf";
    const char* asteroidModelPath2 = "../Data/Models/Asteroids/asteroid2/scene.gltf";
    const char* asteroidModelPath3 = "../Data/Models/Asteroids/meteorite/scene.gltf";
    const char* asteroidModelPath4 = "../Data/Models/Asteroids/asteroid_01/scene.gltf";

    asteroidModels.push_back(getModel(asteroidModelPath1));
    asteroidModels.push_back(getModel(asteroidModelPath2));
    asteroidModels.push_back(getModel(asteroidModelPath3));
    asteroidModels.push_back(getModel(asteroidModelPath4));

    return asteroidModels;
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

shared_ptr<Shader> Engine::GetDefaultEmissiveShader()
{
    return instance.defaultEmissiveShader;
}

shared_ptr<Shader> Engine::GetDefaultBBoxShader()
{
    return instance.defaultBBoxShader;
}

glm::vec2 Engine::getMousePosition()
{
    double xpos, ypos;
    glfwGetCursorPos(instance.window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

glm::vec2 Engine::getMousePositionInUISpace()
{
    glm::vec2 mousePos = getMousePosition();
    glm::vec2 uiScale = glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) / getScreenSize();
    return glm::vec2(0,SCREEN_HEIGHT) + glm::vec2(1,-1)*(mousePos * uiScale);
}

glm::vec2 Engine::getScreenSize()
{
    int width, height;
    glfwGetWindowSize(instance.window, &width, &height);
    return glm::vec2(width, height);
}
