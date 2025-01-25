#pragma once
#include "Common.h"
#include "Sprite.h"

class HealthBar
{
public:
    void create(const glm::vec2& position, const glm::vec2& size, float maxHealth);
    void setHealth(float value);
    void draw();
    void setPosition(const glm::vec2& position) { this->position = position; }
    void setSize(const glm::vec2& size) { this->size = size; }

private:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 backgroundColor;
    glm::vec4 healthColor;

    float maxHealth;
    float currentHealth;

    shared_ptr<Sprite> backgroundSprite;
    shared_ptr<Sprite> foregroundSprite;
};

