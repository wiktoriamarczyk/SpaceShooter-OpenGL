#include "GameObject.h"

void GameObject::update(float deltaTime)
{
    if (isOffScreen() && offScreenDeathEnabled)
    {
        alive = false;
    }
}

bool GameObject::isOffScreen() const
{
    return position.y > screenBoundsY.y || position.y < screenBoundsY.x
        || position.x > screenBoundsX.y || position.x < screenBoundsX.x
        || position.z > screenBoundZ;
}

void GameObject::updateHealth(float value)
{
    currentHealth += value;
    currentHealth = glm::clamp(currentHealth, 0.0f, maxHealth);
    if (currentHealth <= 0.0f)
    {
        alive = false;
    }
}
