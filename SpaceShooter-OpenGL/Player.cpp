#include "Player.h"
#include "Projectile.h"

void Player::create(const Model& model, const Shader& shader, const Model& projectileModel)
{
    ModelObject::create(model, shader);
    rotation.x = 30.f;
    rotation.y = 180.f;
    this->projectileModel = projectileModel.getSelf();
}

void Player::update(float deltaTime)
{
    // movement
    const glm::vec2 boundHorizontal = glm::vec2(-1.25f, 1.25f);
    const glm::vec2 boundVertical = glm::vec2(-0.8f, 1.0f);

    position += movementDirection * speed * deltaTime;

    if (position.x < boundHorizontal.x)
        position.x = boundHorizontal.x;
    else if (position.x > boundHorizontal.y)
        position.x = boundHorizontal.y;

    if (position.y < boundVertical.x)
        position.y = boundVertical.x;
    else if (position.y > boundVertical.y)
        position.y = boundVertical.y;

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
        shootProjectile(glm::vec3(position.x, position.y, -20.0f));
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

glm::vec3 Player::getPosition() const
{
    return position;
}

void Player::shootProjectile(const glm::vec3& targetPosition)
{
    if (shader && projectileModel)
    {
        glm::vec3 projectileStartPos = position + glm::vec3(0.0f, 0.1f, -0.3f);
        shared_ptr<Projectile> newProjectile = make_shared<Projectile>();
        newProjectile->create(projectileStartPos, targetPosition, 5.f, *projectileModel, *shader);
        newProjectile->setScreenBoundZ(20.0f);
        newProjectile->setSize(glm::vec3(0.02f, 0.02f, 0.02f));
        //projectiles.push_back(newProjectile);
        Engine::getInstance().addGameObject(newProjectile);
    }
    else {
        std::cerr << "Shader or model was destroyed or never initialized!" << std::endl;
    }
}