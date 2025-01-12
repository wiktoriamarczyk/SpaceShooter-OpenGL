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
        shootProjectile(glm::vec3(0.0f, 0.0f, -1.0f));
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

void Player::shootProjectile(const glm::vec3& positionToShoot)
{
    if (shader && projectileModel) {
        glm::vec3 projectileStartPos = position + glm::vec3(0.0f, 0.1f, 0.0f);

        std::shared_ptr<Projectile> newProjectile = Projectile::createProjectile(projectileStartPos, positionToShoot, 3.0f, *projectileModel, *shader);

        projectiles.push_back(newProjectile);
        Engine::getInstance().addGameObject(newProjectile);
    }
    else {
        std::cerr << "Shader or model was destroyed or never initialized!" << std::endl;
    }
}