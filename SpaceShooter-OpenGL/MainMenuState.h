#pragma once
#include "GameState.h"
#include "GameObject.h"

class MainMenuState : public GameState
{
public:
    MainMenuState();
    ~MainMenuState() {};
    void update(float deltaTime)override;
    void render()override;
    void onEnter() override;

private:
    bool renderLoadingText = false;
};

