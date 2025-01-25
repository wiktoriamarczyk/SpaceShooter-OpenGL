#include "Player.h"
#include "Projectile.h"

#define WORLD_RANGE 5.0f

void Player::create(const Model& model, const Shader& shader, const Model& projectileModel)
{
    ModelObject::create(model, shader);
    rotation.y = 180.f;
    setSize(glm::vec3(0.5f, 0.5f, 0.5f));
    this->projectileModel = projectileModel.getSelf();

    healthBar = make_shared<HealthBar>();
    healthBar->create(glm::vec2(SCREEN_WIDTH / 2 + 285.0f, SCREEN_HEIGHT - 27.0f), glm::vec2(200.0f, 20.0f), maxHealth);
}

void Player::update(float deltaTime)
{
    move(deltaTime);
    rotateTheHead(deltaTime);
    tilt(deltaTime);

    ModelObject::update(deltaTime);

    // decrease health in time for debugging purposes
    // updateHealth(-2.f * deltaTime);
}

void Player::move(float deltaTime)
{
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
}

void Player::tilt(float deltaTime)
{
    if (movementDirection.x != 0.0f)
    {
        float tiltAmount = 0.5f;
        rotation.z += movementDirection.x * tiltAmount;
        float maxTilt = 30.0f;
        rotation.z = glm::clamp(rotation.z, -maxTilt, maxTilt);
    }
    else
    {
        float tiltRestorationSpeed = 10.0f;
        rotation.z = glm::mix(rotation.z, 0.0f, deltaTime * tiltRestorationSpeed);
    }
}

void Player::rotateTheHead(float deltaTime)
{
    const float ANGLE_OFFSET = 0.f;

    glm::vec2 cursorPosition = Engine::getInstance().getMousePosition();
    glm::vec2 screenSize = Engine::getInstance().getScreenSize();

    // normalize cursor position to range <-1, 1>
    float normalizedX = (cursorPosition.x / screenSize.x) * 2.0f - 1.0f;
    float normalizedY = 1.0f - (cursorPosition.y / screenSize.y) * 2.0f;
    // calculate world position of cursor
    float worldX = normalizedX * WORLD_RANGE;
    float worldY = normalizedY * WORLD_RANGE;

    float cursorDepth = -5.0f;
    glm::vec3 cursorIn3D(normalizedX, normalizedY, cursorDepth);
    glm::vec3 directionToCursor = glm::normalize(cursorIn3D - position);

    float angleY = glm::degrees(atan2(directionToCursor.x, directionToCursor.z));
    float angleX = glm::degrees(atan2(directionToCursor.y, glm::length(glm::vec2(directionToCursor.x, directionToCursor.z))));
    // make sure to rotate shortest way
    float deltaAngleY = angleDifference(rotation.y, angleY) + ANGLE_OFFSET;
    float deltaAngleX = angleDifference(rotation.x, angleX);

    float rotationSpeed = 10.0f;
    rotation.y += deltaAngleY * deltaTime * rotationSpeed;
    rotation.x += deltaAngleX * deltaTime * rotationSpeed;
}


void Player::render()
{
    ModelObject::render();
    healthBar->draw();
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

void Player::onMouseButtonDown(int button)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        glm::vec2 cursorPosition = Engine::getInstance().getMousePosition();
        glm::vec2 screenSize = Engine::getInstance().getScreenSize();

        float normalizedX = (cursorPosition.x / screenSize.x) * 2.0f - 1.0f;
        float normalizedY = 1.0f - (cursorPosition.y / screenSize.y) * 2.0f;

        float worldX = normalizedX * WORLD_RANGE;
        float worldY = normalizedY * WORLD_RANGE;

        float cursorDepth = -20.0f;

        // Tworzymy wektor 3D dla pozycji kursora w przestrzeni œwiata
        glm::vec3 cursorIn3D(worldX, worldY, cursorDepth);

        shootProjectile(cursorIn3D);
    }
}

glm::vec3 Player::getPosition() const
{
    return position;
}

void Player::updateHealth(float value)
{
    ModelObject::updateHealth(value);
    healthBar->setHealth(currentHealth);
}

void Player::shootProjectile(const glm::vec3& targetPosition)
{
    if (shader && projectileModel)
    {
        glm::vec3 projectileStartPos = position + glm::vec3(0.0f, 0.1f, -0.3f);
        shared_ptr<Projectile> newProjectile = make_shared<Projectile>();
        newProjectile->create(projectileStartPos, targetPosition, TEAM::PLAYER, 5.f, *projectileModel, *shader);
        newProjectile->setScreenBoundZ(20.0f);
        newProjectile->setSize(glm::vec3(0.015f, 0.015f, 0.015f));
        //projectiles.push_back(newProjectile);
        Engine::getInstance().addGameObject(newProjectile);
    }
    else {
        std::cerr << "Shader or model was destroyed or never initialized!" << std::endl;
    }
}