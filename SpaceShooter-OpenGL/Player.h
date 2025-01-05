#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();
    void update(float deltaTime) override;
    void render() override;
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;
    ~Player();
};

