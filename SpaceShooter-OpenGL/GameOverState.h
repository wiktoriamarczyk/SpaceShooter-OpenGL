#pragma once
#include "GameState.h"

class GameOverState : public GameState
{
public:
    GameOverState();
    ~GameOverState() {};
    void update(float deltaTime)override;
    void render()override;
    void onEnter() override;
};

