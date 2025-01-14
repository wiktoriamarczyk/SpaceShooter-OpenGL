#include "AsteroidSpawner.h"
#include "Engine.h"

void AsteroidSpawner::create(const vector<shared_ptr<Model>> models, const Shader& shader)
{
    initialObjectCount = 5;
    maxObjectsPerSpawn = 3;

    Spawner::create(models, shader);
}

void AsteroidSpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);
}

void AsteroidSpawner::spawn()
{
    int randomModelIndex = std::rand() % models.size();
    std::shared_ptr<Model> model = models[randomModelIndex];

    if (model)
    {
        glm::vec3 spawnPosition = getRandomSpawnPosition();
        if (!initialized)
        {
            spawnPosition = getRandomInitialSpawnPosition();
        }

        glm::vec3 cameraPosition = Engine::getInstance().getCameraPosition();

        std::shared_ptr<Asteroid> newAsteroid = std::make_shared<Asteroid>();
        newAsteroid->setSize(glm::vec3(0.05f, 0.05f, 0.05f));
        newAsteroid->setPosition(spawnPosition);

        glm::vec3 direction = glm::normalize(cameraPosition - spawnPosition);
        newAsteroid->create(*model, *shader, direction);

        activeObjects.push_back(newAsteroid);
        Engine::getInstance().addGameObject(newAsteroid);
    }
    else
    {
        std::cerr << "Nie uda³o siê za³adowaæ modelu asteroidy!" << std::endl;
    }
}

glm::vec3 AsteroidSpawner::getRandomSpawnPosition() const
{
    const float randomOffsetRangeXY = 1.0;
    // [-1.0, 1.0]
    float randX = -randomOffsetRangeXY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2 * randomOffsetRangeXY));
    float randY = -randomOffsetRangeXY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2 * randomOffsetRangeXY));
    // [-70.0, -50.0]
    float randZ = -70.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 20.0f);

    return glm::vec3(randX, randY, randZ);
}

glm::vec3 AsteroidSpawner::getRandomInitialSpawnPosition() const
{
    const float randomOffsetRangeXY = 1.0;
    // [-1.0, 1.0]
    float randX = -randomOffsetRangeXY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2 * randomOffsetRangeXY));
    float randY = -randomOffsetRangeXY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2 * randomOffsetRangeXY));
    // [-50.0, -15.0]
    float randZ = -50.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 35.0f);

    return glm::vec3(randX, randY, randZ);
}
