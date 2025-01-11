#include "EnemyUnit.h"
#include "Shader.h"
#include "Engine.h"

void EnemyUnit::create(const Model& model, const Shader& shader, const Model& projectileModel)
{
    ModelObject::create(model, shader);
    this->projectileModel = projectileModel.getSelf();

    rotation.x = 0.f;
    rotation.y = 0.f;
    position.y = 1.0f;
    shootCooldown = shootInterval;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void EnemyUnit::update(float deltaTime)
{
    for (int i = 0; i < projectiles.size();)
    {
        if (!projectiles[i]->isAlive())
            i++;
        else
            projectiles.erase(projectiles.begin() + i);
    }

    if (idleTime > 0.0f)
    {
        idleTime -= deltaTime;
        if (idleTime <= 0.0f)
        {
            speed = static_cast<float>(std::rand() % 3 + 1);

            movementDirection.x = (std::rand() % 2 == 0) ? 1.0f : -1.0f;

            float newTargetX;
            do {
                newTargetX = static_cast<float>(std::rand() % 100) / 100.0f * (maxX - minX) + minX;
            } while (std::abs(newTargetX - position.x) < minDistance);

            targetX = newTargetX;
        }
        return;
    }

    position += glm::vec3(movementDirection.x * speed * deltaTime, 0.f, 0.f);

    if ((movementDirection.x > 0 && position.x >= targetX) || (movementDirection.x < 0 && position.x <= targetX))
    {
        setRandomIdleTime();
    }

    // Aktualizacja cooldownu strza³u
    shootCooldown -= deltaTime;
    if (shootCooldown <= 0.0f)
    {
        shootProjectile(); // Strzelanie pociskiem
        shootCooldown = shootInterval; // Reset cooldownu
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
}

void EnemyUnit::shootProjectile()
{
    if (shader) {
        shared_ptr<Projectile> newProjectile = make_shared<Projectile>();
        glm::vec3 projectileStartPos = position + glm::vec3(0.0f, -0.1f, 0.0f);
        // Move projectile down
        glm::vec3 projectileDirection = glm::vec3(0.0f, -1.0f, 0.0f);

        newProjectile->create(projectileStartPos, projectileDirection, 1.0f, *projectileModel, *shader);
        newProjectile->setSize(glm::vec3(0.05f, 0.05f, 0.05f));
        projectiles.push_back(newProjectile);
        Engine::getInstance().addGameObject(newProjectile);
    }
    else {
        std::cerr << "Shader was destroyed or never initialized!" << std::endl;
    }
}