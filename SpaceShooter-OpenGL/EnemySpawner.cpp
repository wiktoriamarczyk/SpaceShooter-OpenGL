#include "EnemySpawner.h"
#include "Engine.h"

void EnemySpawner::create(const Model& enemyModel, const Shader& enemyShader, const Model& projectileModel) {
    this->enemyModel = std::make_shared<Model>(enemyModel);
    this->enemyShader = std::make_shared<Shader>(enemyShader);
    this->projectileModel = std::make_shared<Model>(projectileModel);

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void EnemySpawner::update(float deltaTime) {

    timeSinceLastSpawn += deltaTime;

    if (timeSinceLastSpawn >= spawnInterval && activeEnemies.size() < maxEnemies) {
        spawnEnemy();
        timeSinceLastSpawn = 0.0f;
    }

    for (int i = 0; i < activeEnemies.size();) {
        if (!activeEnemies[i]->isAlive()) {
            activeEnemies.erase(activeEnemies.begin() + i);
        }
        else {
            i++;
        }
    }
}

void EnemySpawner::spawnEnemy() {
    if (enemyModel && enemyShader && projectileModel) {

        glm::vec3 spawnPosition = getRandomSpawnPosition();

        std::shared_ptr<EnemyUnit> newEnemy = std::make_shared<EnemyUnit>();
        newEnemy->create(*enemyModel, *enemyShader, *projectileModel);
        newEnemy->setPosition(spawnPosition);

        activeEnemies.push_back(newEnemy);
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

void EnemySpawner::render()
{

}

