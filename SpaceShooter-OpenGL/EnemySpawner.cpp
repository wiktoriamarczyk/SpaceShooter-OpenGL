#include "EnemySpawner.h"
#include "Engine.h"

void EnemySpawner::create(const vector<shared_ptr<Model>> models, const Shader& shader, const Model& projectileModel)
{
    this->projectileModel = projectileModel.getSelf();
    this->models = models;

    spawnInterval = 3.0f;
    maxActiveObjects = 3;

    Spawner::create(shader);
}

void EnemySpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);
    eraseInactiveObjects();
}

void EnemySpawner::spawn()
{
    int randomModelIndex = std::rand() % models.size();
    std::shared_ptr<Model> model = models[randomModelIndex];

    if (model && projectileModel) {

        glm::vec3 spawnPosition = getRandomSpawnPosition();

        std::shared_ptr<EnemyUnit> newEnemy = std::make_shared<EnemyUnit>();
        newEnemy->create(*model, *shader, *projectileModel);
        newEnemy->setPosition(spawnPosition);

        activeObjects.push_back(newEnemy);
        Engine::getInstance().addGameObject(newEnemy);
    }
    else {
        std::cerr << "Model, shader lub projectileModel nie zosta³y poprawnie zainicjalizowane!" << std::endl;
    }
}

glm::vec3 EnemySpawner::getRandomSpawnPosition() const
{
    int side = std::rand() % 4;
    float x, y;

    switch (side)
    {
    case 0:
        x = static_cast<float>(std::rand() % 200 - 100) / 100.0f * windowHalfSize;
        y = windowHalfSize + spawnMargin;
        break;

    case 1:
        x = static_cast<float>(std::rand() % 200 - 100) / 100.0f * windowHalfSize;
        y = -windowHalfSize - spawnMargin;
        break;

    case 2:
        x = -windowHalfSize - spawnMargin;
        y = static_cast<float>(std::rand() % 200 - 100) / 100.0f * windowHalfSize;
        break;

    case 3:
        x = windowHalfSize + spawnMargin;
        y = static_cast<float>(std::rand() % 200 - 100) / 100.0f * windowHalfSize;
        break;
    }

    return glm::vec3(x, y, -3.0f);
}