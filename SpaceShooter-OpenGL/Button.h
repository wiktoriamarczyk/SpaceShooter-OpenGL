#pragma once

#include "Common.h"
#include "GameObject.h"
#include "Sprite.h"

class Button : GameObject
{
public:
    void create(const glm::vec2& position, const glm::vec2& size, string text, function<void()> callback);
    void update(float deltaTime) override;
    void render() override;
    void onMouseButtonDown(int button) override;
    void onMouseButtonUp(int button) override;

private:
    string text;
    glm::vec4 backgroundColor = glm::vec4(1,1,1,1);
    glm::vec4 hoverColor = glm::vec4(0.8, 0.8, 0.8, 1);
    glm::vec4 clickColor = glm::vec4(0.6, 0.6, 0.6, 1);

    shared_ptr<Sprite> sprite;
    function<void()> callback;
    bool hover = false;
    bool pressed = false;

    bool isMouseOver();
};

