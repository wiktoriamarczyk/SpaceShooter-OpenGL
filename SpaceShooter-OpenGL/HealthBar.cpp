#include "HealthBar.h"
#include "Engine.h"

void HealthBar::create(const glm::vec2& position, const glm::vec2& size, float maxHealth)
{
    backgroundColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    healthColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    this->position = position;
    this->size = size;
    this->maxHealth = this->currentHealth = maxHealth;

    shared_ptr<Texture> whiteTexture = Engine::getTexture("../Data/Textures/white-tex.png");
    if (!whiteTexture)
    {
        std::cout << "Couldn't load healthbar texture!" << std::endl;
        return;
    }
    backgroundSprite = Sprite::create(*whiteTexture, position, size);
    foregroundSprite = Sprite::create(*whiteTexture, position, size);
}


void HealthBar::setHealth(float value)
{
    currentHealth = glm::clamp(value, 0.0f, maxHealth);
}

void HealthBar::draw()
{
    backgroundSprite->setColor(backgroundColor);
    foregroundSprite->setColor(healthColor);

    backgroundSprite->setPosition(position);
    backgroundSprite->setSize(size);

    float healthPercentage = glm::clamp(currentHealth / maxHealth, 0.0f, 1.0f);
    glm::vec2 healthBarSize = glm::vec2(size.x * healthPercentage, size.y);

    glm::vec2 foregroundPosition = position;
    foregroundPosition.x = position.x + (size.x - healthBarSize.x) / 2;

    foregroundSprite->setPosition(foregroundPosition);
    foregroundSprite->setSize(healthBarSize);

    foregroundSprite->draw();
    backgroundSprite->draw();
}