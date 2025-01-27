#include "EnemyUnit.h"
#include "Shader.h"
#include "Engine.h"

void EnemyUnit::create(const Model& model, const Shader& shader, const Model& projectileModel)
{
    ModelObject::create(model, shader);
    this->projectileModel = projectileModel.getSelf();

    position = glm::vec3(0.0f, 1.0f, -2.0f);
    shootCooldown = shootInterval;
    remainingShots = 0;
    postShotCooldown = 0.0f;
    targetPosition = glm::vec3(position.x, position.y, 0.0f);

    setSize(glm::vec3(0.4f, 0.4f, 0.4f));
    setRandomIdleTime();

    srand(static_cast<unsigned>(time(nullptr)));
}

void EnemyUnit::update(float deltaTime)
{
    glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();
    glm::vec3 directionToPlayer = glm::normalize(playerPosition - position);

    float angleY = glm::degrees(atan2(directionToPlayer.x, directionToPlayer.z));
    float angleX = glm::degrees(atan2(directionToPlayer.y, glm::length(glm::vec2(directionToPlayer.x, directionToPlayer.z))));

    rotation.y = glm::mix(rotation.y, angleY, deltaTime * 2.0f);
    rotation.x = glm::mix(rotation.x, -angleX, deltaTime * 2.0f);

    if (idleTime > 0.0f)
    {
        idleTime -= deltaTime;

        if (idleTime <= 0.0f)
        {
            setRandomTargetPosition();
            remainingShots = rand() % 10 + 1;
            shootCooldown = shootInterval;
        }

        if (remainingShots > 0)
        {
            shootCooldown -= deltaTime;

            if (shootCooldown <= 0.0f)
            {
                shootProjectile(playerPosition);
                shootCooldown = shootInterval;
                remainingShots--;
            }
        }

        return;
    }

    if (remainingShots == 0 && postShotCooldown > 0.0f)
    {
        postShotCooldown -= deltaTime;

        if (postShotCooldown <= 0.0f)
        {

            setRandomTargetPosition();
            currentSpeed = 0.0f;
        }

        return;
    }

    if (remainingShots > 0)
    {
        shootCooldown -= deltaTime;

        if (shootCooldown <= 0.0f)
        {
            shootProjectile(playerPosition);
            shootCooldown = shootInterval;
            remainingShots--;
        }
    }
    else
    {
        if (postShotCooldown <= 0.0f)
        {
            glm::vec3 direction = glm::normalize(targetPosition - position);

            if (currentSpeed < speed)
            {
                currentSpeed += acceleration * deltaTime;
                currentSpeed = glm::min(currentSpeed, speed);
            }

            float distanceToTarget = glm::distance(position, targetPosition);
            if (distanceToTarget < decelerationDistance)
            {
                currentSpeed = glm::max(currentSpeed * (distanceToTarget / decelerationDistance), 0.5f);
            }

            position += direction * currentSpeed * deltaTime;

            if (distanceToTarget < 0.05f)
            {
                setRandomIdleTime();
            }

            position.x = glm::clamp(position.x, minX, maxX);
            position.y = glm::clamp(position.y, minY, maxY);
        }
        else
        {

            postShotCooldown -= deltaTime;
        }
    }

    ModelObject::update(deltaTime);
}

void EnemyUnit::render()
{
    ModelObject::render();
}

void EnemyUnit::setRandomIdleTime()
{
    idleTime = static_cast<float>(rand() % 3 + 1);
    postShotCooldown = 0.5f;
}

void EnemyUnit::setRandomTargetPosition()
{
    float newTargetX, newTargetY, newTargetZ;

    do {
        newTargetX = static_cast<float>(rand() % 600 - 300) / 100.0f;
        newTargetY = static_cast<float>(rand() % 400 - 200) / 100.0f;
        newTargetZ = static_cast<float>(rand() % 3 + 6) * -1.0f;
    } while (glm::distance(glm::vec3(newTargetX, newTargetY, 0.0f), position) < minDistance);

    targetPosition = glm::vec3(newTargetX, newTargetY, newTargetZ);
}

void EnemyUnit::shootProjectile(const glm::vec3& playerPosition)
{
    float distanceToPlayer = std::abs(position.z - playerPosition.z);

    if (distanceToPlayer > 40.0f)
    {
        return;
    }

    if (shader && projectileModel)
    {
        glm::vec3 projectileStartPos = position;
        shared_ptr<Projectile> newProjectile = make_shared<Projectile>();
        newProjectile->create(projectileStartPos, playerPosition, TEAM::ENEMY, 5.0f, *projectileModel);
        newProjectile->setSize(glm::vec3(0.01f, 0.01f, 0.01f));
        Engine::getInstance().addGameObject(newProjectile);
        Engine::getInstance().playSound(SHOOT_SOUND_PATH);
    }
    else {
        std::cerr << "Shader or model was destroyed or never initialized!" << std::endl;
    }
}
