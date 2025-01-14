#include "GameObject.h"

void GameObject::update(float deltaTime)
{
    if (isOffScreen() && offScreenDeathEnabled)
    {
        alive = false;
        std::cout << "Object off screen" << std::endl;
        // NOT WORKING?
    }
}

// TODO: Fix magic numbers
bool GameObject::isOffScreen() const
{
    return position.y > 1.5f || position.y < -1.5f || position.x > 1.5f || position.x < -1.5f;
}