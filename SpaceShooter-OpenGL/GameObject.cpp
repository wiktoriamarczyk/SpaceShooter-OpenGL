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