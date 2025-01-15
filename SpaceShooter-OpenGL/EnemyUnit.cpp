#include "EnemyUnit.h"
#include "Shader.h"
#include "Engine.h"

void EnemyUnit::create(const Model& model, const Shader& shader, const Model& projectileModel)
{
    ModelObject::create(model, shader);
    this->projectileModel = projectileModel.getSelf();

    rotation.x = 0.f;
    rotation.y = 0.f;
    position = glm::vec3(0.0f, 1.0f, -2.0f);
    shootCooldown = shootInterval;

    setSize(glm::vec3(0.1f, 0.1f, 0.1f));

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    remainingShots = 0;
    setRandomIdleTime();
    postShotCooldown = 0.0f;

    targetPosition = glm::vec3(position.x, position.y, 0.0f);
}

void EnemyUnit::update(float deltaTime)
{
    glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();

    glm::vec3 directionToPlayer = glm::normalize(playerPosition - position);

    float angleY = std::atan2(directionToPlayer.x, directionToPlayer.z);
    float angleX = std::atan2(directionToPlayer.y, glm::length(glm::vec2(directionToPlayer.x, directionToPlayer.z)));

    rotation.y = glm::mix(rotation.y, angleY * 5.0f, deltaTime * 2.0f);  
    rotation.x = glm::mix(rotation.x, -angleX * 5.0f, deltaTime * 2.0f);  

    for (int i = 0; i < projectiles.size();)
    {
        if (!projectiles[i]->isAlive())
            projectiles.erase(projectiles.begin() + i);
        else
            i++;
    }

    if (idleTime > 0.0f)
    {
        idleTime -= deltaTime;

        if (idleTime <= 0.0f)
        {
            setRandomTargetPosition(); 
            remainingShots = std::rand() % 10 + 1; 
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
    idleTime = static_cast<float>(std::rand() % 3 + 1);
    postShotCooldown = 1.0f;
}

void EnemyUnit::setRandomTargetPosition()
{
    float newTargetX, newTargetY, newTargetZ;

    do {
        newTargetX = static_cast<float>(std::rand() % 200 - 100) / 100.0f; 
        newTargetY = static_cast<float>(std::rand() % 200 - 100) / 100.0f;
        newTargetZ = static_cast<float>(std::rand() % 5 + 3) * -1.0f;
    } while (glm::distance(glm::vec3(newTargetX, newTargetY, 0.0f), position) < minDistance);

    targetPosition = glm::vec3(newTargetX, newTargetY, newTargetZ);
}

void EnemyUnit::shootProjectile(const glm::vec3& playerPosition)
{
    if (shader && projectileModel) {
        glm::vec3 projectileStartPos = position /*+ glm::vec3(0.0f, 0.0f, 0.0f)*/; 

        std::shared_ptr<Projectile> newProjectile = Projectile::createProjectile(projectileStartPos, playerPosition, 3.0f, *projectileModel, *shader);

        projectiles.push_back(newProjectile);
        Engine::getInstance().addGameObject(newProjectile);
    }
    else {
        std::cerr << "Shader or model was destroyed or never initialized!" << std::endl;
    }
}
