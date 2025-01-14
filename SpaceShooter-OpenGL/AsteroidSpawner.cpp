#include "AsteroidSpawner.h"
#include "Engine.h"

void AsteroidSpawner::create(const vector<shared_ptr<Model>> models, const Shader& shader)
{
    this->models = models;

    initialObjectCount = 5;
    maxObjectsPerSpawn = 3;

    Spawner::create(shader);
}

void AsteroidSpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);
    eraseInactiveObjects();
}

void AsteroidSpawner::spawn()
{
    int randomModelIndex = std::rand() % models.size();
    shared_ptr<Model> model = models[randomModelIndex];

    if (model)
    {
        glm::vec3 spawnPosition = getRandomSpawnPosition();
        if (!initialized)
        {
            spawnPosition = getRandomInitialSpawnPosition();
        }

        glm::vec3 cameraPosition = Engine::getInstance().getCameraPosition();

        shared_ptr<Asteroid> newAsteroid = std::make_shared<Asteroid>();
        newAsteroid->setSize(glm::vec3(0.05f, 0.05f, 0.05f));
        newAsteroid->setPosition(spawnPosition);

        glm::vec3 direction = glm::normalize(cameraPosition - spawnPosition);
        newAsteroid->create(*model, *shader, direction);

        activeObjects.push_back(newAsteroid);
        Engine::getInstance().addGameObject(newAsteroid);
    }
    else
    {
        std::cerr << "Nie uda�o si� za�adowa� modelu asteroidy!" << std::endl;
    }
}

glm::vec3 AsteroidSpawner::getRandomSpawnPosition() const
{
    float randX = randomFloat(-1.0f, 1.0f);
    float randY = randomFloat(-1.0f, 1.0f);
    float randZ = randomFloat(-70.0f, -50.0f);

    return glm::vec3(randX, randY, randZ);
}

glm::vec3 AsteroidSpawner::getRandomInitialSpawnPosition() const
{
    float randX = randomFloat(-1.0f, 1.0f);
    float randY = randomFloat(-1.0f, 1.0f);
    float randZ = randomFloat(-50.0f, -15.0f);

    return glm::vec3(randX, randY, randZ);
}
