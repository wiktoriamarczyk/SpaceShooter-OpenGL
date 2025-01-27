#pragma once
#include "Common.h"
#include "GameObject.h"

class InGameState;

class GameState
{
public:
    GameState(GAME_STATE stateID);
    virtual void update(float deltaTime);
    virtual void render();
    virtual void onEnter();
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual void onMouseButtonDown(int button);
    virtual void onMouseButtonUp(int button);
    virtual InGameState* asInGameState() { return nullptr; }
    GAME_STATE getState() { return stateID; }
    void addGameObject(shared_ptr<GameObject> gameObject) { gameObjects.push_back(gameObject); }

protected:
    vector<shared_ptr<GameObject>> gameObjects;
    static bool gameOver;

private:
    const GAME_STATE stateID = GAME_STATE::UNKNOWN;
};

