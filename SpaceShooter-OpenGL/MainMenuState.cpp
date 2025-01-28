#include "MainMenuState.h"
#include "Button.h"
#include "Engine.h"

MainMenuState::MainMenuState() : GameState(GAME_STATE::MENU) {}

void MainMenuState::update(float deltaTime)
{
    GameState::update(deltaTime);
}

void MainMenuState::render()
{
    GameState::render();

    Engine::getInstance().renderText("SPACE SHOOTER", glm::vec2(SCREEN_WIDTH / 2 - 180.f, SCREEN_HEIGHT / 2 + 200.f));

    if (renderLoadingText)
        Engine::getInstance().renderText("Loading...", glm::vec2(SCREEN_WIDTH / 2 - 125.f, SCREEN_HEIGHT / 2 + 100.f), 0.5f, glm::vec3(0, 0.4, 1));
}

void MainMenuState::onEnter()
{
    GameState::onEnter();
    renderLoadingText = false;

    glm::vec2 buttonSize = glm::vec2(200.0f, 50.0f);
    shared_ptr<Button> buttonPlay = make_shared<Button>();
    buttonPlay->create(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), buttonSize, "PLAY", [this]()
        {
            renderLoadingText = true;
            Engine::getInstance().changeGameState(GAME_STATE::IN_GAME);
        });

    gameObjects.push_back(buttonPlay);

    shared_ptr<Button> buttonExit = make_shared<Button>();
    buttonExit->create(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 2 * buttonSize.y), buttonSize, "EXIT", [this]()
        {
            Engine::getInstance().exitGame();
        });

    gameObjects.push_back(buttonExit);
}
