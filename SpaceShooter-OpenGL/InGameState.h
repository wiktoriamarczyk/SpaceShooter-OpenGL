#pragma once
#include "GameState.h"
#include "Player.h"

class InGameState : public GameState
{
public:
    friend class Engine;
    InGameState();
    ~InGameState() {};
    virtual InGameState* asInGameState() override { return this; }
    void update(float deltaTime)override;
    void render()override;
    void onEnter() override;
    void onKeyDown(int key) override;

protected:
    weak_ptr<Player> player;
};

