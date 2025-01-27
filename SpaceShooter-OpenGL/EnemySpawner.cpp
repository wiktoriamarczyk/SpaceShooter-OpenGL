#include "EnemySpawner.h"
#include "Engine.h"

int EnemySpawner::removedEnemies = 0;

void EnemySpawner::create(const vector<shared_ptr<Model>>& models, const Shader& shader, const Model& projectileModel)
{
    this->projectileModel = projectileModel.getSelf();
    this->models = models;

    spawnInterval = 3.0f;
    maxActiveObjects = 1;

    Spawner::create(shader);
}

void EnemySpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);
    eraseInactiveObjects();
}

void EnemySpawner::spawn()
{
    int randomModelIndex = rand() % models.size();
    shared_ptr<Model> model = models[randomModelIndex];

    if (model && projectileModel) {

        glm::vec3 spawnPosition = getRandomSpawnPosition();

        shared_ptr<EnemyUnit> newEnemy = make_shared<EnemyUnit>();
        newEnemy->create(*model, *shader, *projectileModel);
        newEnemy->setPosition(spawnPosition);

        activeObjects.push_back(newEnemy);
        Engine::getInstance().addGameObject(newEnemy);
    }
    else {
        std::cerr << "Enemy or projectile model initialization failed" << std::endl;
    }
}

glm::vec3 EnemySpawner::getRandomSpawnPosition() const
{
    float x = static_cast<float>(rand() % 200 - 100) / 100.0f;
    float y = static_cast<float>(rand() % 200 - 100) / 100.0f;

    return glm::vec3(x, y, -50.0f);
}

void EnemySpawner::eraseInactiveObjects()
{
    for (int i = 0; i < activeObjects.size();)
    {
        if (!activeObjects[i]->isAlive())
        {
            activeObjects.erase(activeObjects.begin() + i);
            EnemySpawner::removedEnemies++;
        }
        else
        {
            i++;
        }
    }

    if (EnemySpawner::removedEnemies > 0)
    {
        maxActiveObjects = std::min(maxActiveObjects.value_or(0) + removedEnemies, maxPossibleObjects);
    }
}