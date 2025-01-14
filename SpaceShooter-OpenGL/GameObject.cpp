#include "GameObject.h"

void GameObject::update(float deltaTime)
{
    if (isOffScreen() && offScreenDeathEnabled)
    {
        alive = false;
        std::cout << "Object " << name << " off screen" << std::endl;
        // NOT WORKING?
    }
}

// TODO: Fix magic numbers
bool GameObject::isOffScreen() const
{
    return position.y > screenBoundsY.y || position.y < screenBoundsY.x
        || position.x > screenBoundsX.y || position.x < screenBoundsX.x
        || position.z > screenBoundZ;
}