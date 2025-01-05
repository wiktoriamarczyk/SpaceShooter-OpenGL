#include "Engine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "TestCube.h"

Engine Engine::instance;

using VertexDefinitionElement = VertexArrayObject::VertexDefinitionElement;

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

    glfwSetKeyCallback(window, processInputCallback);

    // Create default objects
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
    defaultVAO->create(*defaultVBO, VertexArrayObject::POSITION | VertexArrayObject::TEXTURE_COORD);

    defaultShader = make_shared<Shader>();
    if (!defaultShader->load("../Data/shaders/sprite_shader.vs", "../Data/shaders/sprite_shader.fs"))
        return false;
    defaultShader->use();

    // set orthographic projection matrix
    auto ortho = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    defaultShader->setMat4("projection", ortho);
    // set identity matrix for view
    defaultShader->setMat4("view", glm::identity<glm::mat4>());

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);

    if (WIREFRAME_MODE)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    createGameObjects();

    return true;
}

void Engine::createGameObjects()
{
    // TODO: Create game objects here
    shared_ptr<TestCube> testCube = make_shared<TestCube>();
    gameObjects.push_back(testCube);
}

shared_ptr<Texture> Engine::getTexture(const char* path)
{
    auto foundTexture = std::find_if(instance.textures.begin(), instance.textures.end(), [path](const shared_ptr<Texture>& texture)
        {
            return texture->getPath() == path;
        });

    if (foundTexture != instance.textures.end())
        return *foundTexture;

    auto texture = Texture::create(path);
    if (!texture)
        return nullptr;

    instance.textures.push_back(texture);
    return texture;
}

void Engine::update(float deltaTime)
{
    for (auto& gameObject : gameObjects)
    {
        gameObject->update(deltaTime);
    }
}

void Engine::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer and depth buffer

    for (auto& gameObject : gameObjects)
    {
        gameObject->render();
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
}

void Engine::run()
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

shared_ptr<Shader> Engine::GetDefaultShader()
{
    return instance.defaultShader;
}