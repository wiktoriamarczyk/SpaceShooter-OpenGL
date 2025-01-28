#include "GameOverState.h"
#include "Common.h"
#include "Engine.h"
#include "Button.h"
#include "EnemySpawner.h"

GameOverState::GameOverState() : GameState(GAME_STATE::GAME_OVER) {}

void GameOverState::update(float deltaTime)
{
    GameState::update(deltaTime);
}

void GameOverState::render()
{
    GameState::render();

    Engine::getInstance().renderText("GAME OVER", glm::vec2(SCREEN_WIDTH / 2 - 150.f, SCREEN_HEIGHT / 2 + 200.f));
    string scoreText = "Score: " + to_string(EnemySpawner::getRemovedEnemiesCount());
    Engine::getInstance().renderText(scoreText, glm::vec2(SCREEN_WIDTH / 2 - 100.f, SCREEN_HEIGHT / 2 + 100.f), 0.5f, glm::vec3(0, 0.4, 1));
}

void GameOverState::onEnter()
{
    GameState::onEnter();

    glm::vec2 buttonSize = glm::vec2(200.0f, 50.0f);
    shared_ptr<Button> buttonReturn = make_shared<Button>();
    buttonReturn->create(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), buttonSize, "MENU", [this]()
        {
            Engine::getInstance().changeGameState(GAME_STATE::MENU);
        });

    gameObjects.push_back(buttonReturn);

    shared_ptr<Button> buttonExit = make_shared<Button>();
    buttonExit->create(glm::vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 2 * buttonSize.y), buttonSize, "EXIT", [this]()
        {
            Engine::getInstance().exitGame();
        });

    gameObjects.push_back(buttonExit);
}
