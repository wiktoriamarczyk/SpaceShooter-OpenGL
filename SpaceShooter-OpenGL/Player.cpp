#include "Player.h"
#include "Projectile.h"

void Player::create(const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);
    rotation.x = 30.f;
    rotation.y = 180.f;
}

void Player::update(float deltaTime)
{
    // movement
    position += glm::vec3(movementDirection * speed * deltaTime, 0.f);
    ModelObject::update(deltaTime);
}

void Player::render()
{
    ModelObject::render();
}

void Player::onKeyDown(int key)
{
    if (key == GLFW_KEY_W)
    {
        movementDirection.y = 1.f;
    }
    else if (key == GLFW_KEY_S)
    {
        movementDirection.y = -1.f;
    }
    else if (key == GLFW_KEY_A)
    {
        movementDirection.x = -1.f;
    }
    else if (key == GLFW_KEY_D)
    {
        movementDirection.x = 1.f;
    }
    else if (key == GLFW_KEY_SPACE)
    {
        // Shoot
    }
}

void Player::onKeyUp(int key)
{
    if (key == GLFW_KEY_W)
    {
        movementDirection.y = 0.f;
    }
    else if (key == GLFW_KEY_S)
    {
        movementDirection.y = 0.f;
    }
    else if (key == GLFW_KEY_A)
    {
        movementDirection.x = 0.f;
    }
    else if (key == GLFW_KEY_D)
    {
        movementDirection.x = 0.f;
    }
}