#include <ft2build.h>
#include FT_FREETYPE_H

#include "Engine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Model.h"
#include "ModelObject.h"
#include "Player.h"
#include "PointLight.h"
#include "MainMenuState.h"
#include "InGameState.h"
#include "GameOverState.h"

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
        currentState->onKeyDown(key);
    }
    else if (action == GLFW_RELEASE)
    {
        currentState->onKeyUp(key);

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
        currentState->onMouseButtonDown(button);
    }
    else if (action == GLFW_RELEASE)
    {
        currentState->onMouseButtonUp(button);
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
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (WIREFRAME_MODE)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (!createDefaultResources())
    {
        std::cout << "Failed to create default resources" << std::endl;
        return false;
    }

    music.openFromFile(MUSIC_PATH);
    music.setVolume(25);
    music.setLoop(true);
    music.play();

    if (!freeTypeInit())
    {
        std::cout << "Failed to initialize FreeType" << std::endl;
        return false;
    }

    setCustomCursor();

    return true;
}

bool Engine::freeTypeInit()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, "../Data/BRLNSB.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    textVBO = make_shared<VertexBuffer>();
    textVBO->create(nullptr, sizeof(float) * 5, 6, GL_DYNAMIC_DRAW); // 6 vertices, 4 floats per vertex

    textVAO = make_shared<VertexArrayObject>();
    textVAO->create(*textVBO, VertexDefinitionElement::POSITION | VertexDefinitionElement::TEXTURE_COORD);

    // destroy FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

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

    allStates.push_back(make_unique<MainMenuState>());
    allStates.push_back(make_unique<InGameState>());
    allStates.push_back(make_unique<GameOverState>());
    changeGameState(GAME_STATE::MENU);

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
    defaultSpriteShader = Shader::create(VS_SPRITE_FILE_NAME, FS_SPRITE_FILE_NAME);
    if (!defaultSpriteShader)
        return false;
    defaultSpriteShader->use();

    // set orthographic projection matrix
    auto ortho = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT));
    defaultSpriteShader->setMat4("projection", ortho);
    defaultSpriteShader->setMat4("view", glm::identity<glm::mat4>());


    // Init default model resources
    defaultModelShader = Shader::create(VS_FILE_NAME, FS_FILE_NAME);
    if (!defaultModelShader)
        return false;
    defaultModelShader->use();

    // set perspective projection matrix
    auto projection = projectionMatrix = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
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
    defaultEmissiveShader = Shader::create(VS_EMISSIVE_FILE_NAME, FS_EMISSIVE_FILE_NAME);
    if (!defaultEmissiveShader)
        return false;
    defaultEmissiveShader->use();
    defaultEmissiveShader->setMat4("projection", projection);
    defaultEmissiveShader->setMat4("view", glm::identity<glm::mat4x4>());


    // Init default bounding box shader
    defaultBBoxShader = Shader::create(VS_BBOX_FILE_NAME, FS_BBOX_FILE_NAME);
    if (!defaultBBoxShader)
        return false;
    defaultBBoxShader->use();
    defaultBBoxShader->setMat4("projection", projection);
    defaultBBoxShader->setMat4("view", glm::identity<glm::mat4x4>());
    defaultBBoxShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));


    // Init default text shader
    defaultTextShader = Shader::create(VS_TEXT_FILE_NAME, FS_TEXT_FILE_NAME);
    if (!defaultTextShader)
        return false;
    defaultTextShader->use();
    defaultTextShader->setMat4("projection", ortho);


    // Draw background sprite
    shared_ptr<Texture> backgroundTexture = getTexture(BACKGROUND_PATH);
    background = Sprite::create(*backgroundTexture, glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void Engine::addGameObject(shared_ptr<GameObject> gameObject)
{
    currentState->addGameObject(gameObject);
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
    if (currentState->getState() != GAME_STATE::IN_GAME)
        return glm::vec3(0.0f);

    if (auto player = currentState->asInGameState()->player.lock())
    {
        return player->getPosition();
    }
    return glm::vec3(0.0f);
}

void Engine::setPlayerHealth(float newHealth)
{
    auto player = currentState->asInGameState()->player.lock();
    if (!player)
        return;
    player->updateHealth(newHealth);
}

void Engine::renderText(string text, glm::vec2 position, float scale, glm::vec3 color)
{
    defaultTextShader->use();
    defaultTextShader->setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    textVAO->bind();
    glDisable(GL_DEPTH_TEST);

    // iterate through all characters
    string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        float xpos = position.x + ch.Bearing.x * scale;
        float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][5] = {
            { xpos,     ypos + h,   0 , 0.0f, 0.0f },
            { xpos,     ypos,       0 , 0.0f, 1.0f },
            { xpos + w, ypos,       0 , 1.0f, 1.0f },

            { xpos,     ypos + h,   0 , 0.0f, 0.0f },
            { xpos + w, ypos,       0 , 1.0f, 1.0f },
            { xpos + w, ypos + h,   0 , 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        textVBO->updateData(vertices, sizeof(vertices[0][0])*5, 6);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    textVAO->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
}

void Engine::changeGameState(GAME_STATE newState)
{
    if (initializing)
        initializing = false;
    else
        render();

    for (int i = 0; i < allStates.size(); ++i)
    {
        if (allStates[i]->getState() == newState)
        {
            currentState = allStates[i].get();
            currentState->onEnter();
            return;
        }
    }
}

void Engine::exitGame()
{
    glfwSetWindowShouldClose(window, true);
}

void Engine::playSound(const char* path)
{
    if (soundBuffers.find(path) == soundBuffers.end())
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(path))
        {
            std::cerr << "Failed to load sound file: " << path << std::endl;
            return;
        }

        soundBuffers[path] = std::move(buffer);
    }

    auto sound = make_shared<sf::Sound>();
    sound->setBuffer(soundBuffers[path]);
    sound->setVolume(70);
    sound->play();

    activeSounds.push_back(sound);
}

void Engine::update(float deltaTime)
{
    currentState->update(deltaTime);

    activeSounds.erase(
        std::remove_if(
            activeSounds.begin(),
            activeSounds.end(),
            [](const shared_ptr<sf::Sound>& sound) { return sound->getStatus() == sf::Sound::Stopped; }
        ),
        activeSounds.end()
    );
}

void Engine::render()
{
    // clear the color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    background->draw();
    // clear the depth buffer after drawing the background
    glClear(GL_DEPTH_BUFFER_BIT);

    currentState->render();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
}

glm::vec3 Engine::getMouseWorldRayDirection()const
{
    glm::vec2 mouse_pos = getMousePositionInUISpace();
    glm::vec3 ray_nds;

    ray_nds.x = (2.0f * mouse_pos.x) / SCREEN_WIDTH - 1.0f;
    ray_nds.y = 1.0f - (2.0f * mouse_pos.y) / SCREEN_HEIGHT;
    ray_nds.y = -ray_nds.y;
    ray_nds.z = 1.0f;

    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
    glm::vec4 ray_wrld = glm::inverse(projectionMatrix) * ray_clip;
    // don't forget to normalize the vector at some point
    return glm::normalize(ray_wrld);
};

glm::vec3 Engine::getMouseWorldPosOnTestPlane(float TestPlaneZ)const
{
    Plane testPlane(glm::vec3(-1.0f, -1.0f, TestPlaneZ), glm::vec3(1.0f, -1.0f, TestPlaneZ), glm::vec3(-1.0f, 1.0f, TestPlaneZ));

    if (auto point = testPlane.rayIntersection(glm::vec3(0.0f, 0.0f, 0.0f), getMouseWorldRayDirection()))
        return *point;

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
    return glm::vec2(0, SCREEN_HEIGHT) + glm::vec2(1, -1) * (mousePos * uiScale);
}

glm::vec2 Engine::getScreenSize()
{
    int width, height;
    glfwGetWindowSize(instance.window, &width, &height);
    return glm::vec2(width, height);
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