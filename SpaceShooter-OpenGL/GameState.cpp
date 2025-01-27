#include "GameState.h"

bool GameState::gameOver = false;

GameState::GameState(GAME_STATE id) : stateID(id) {}

void GameState::update(float deltaTime)
{
    for (int i = 0; i < gameObjects.size();)
    {
        if (gameObjects[i]->isAlive())
            gameObjects[i++]->update(deltaTime);
        else
            gameObjects.erase(gameObjects.begin() + i);
    }
}

void GameState::render()
{
    for (int i = 0; i < gameObjects.size(); ++i)
    {
        gameObjects[i]->render();
    }
}

void GameState::onEnter()
{
    gameObjects.clear();
}

void GameState::onKeyDown(int key)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->onKeyDown(key);
    }
}

void GameState::onKeyUp(int key)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->onKeyUp(key);
    }
}

void GameState::onMouseButtonDown(int button)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->onMouseButtonDown(button);
    }
}

void GameState::onMouseButtonUp(int button)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->onMouseButtonUp(button);
    }
}
