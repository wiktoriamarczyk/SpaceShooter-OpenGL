#include "AsteroidSpawner.h"
#include "Engine.h"
#include "Shader.h"

void AsteroidSpawner::create(const Model& model, const Shader& shader)
{
    this->model = std::make_shared<Model>(model);
    this->shader = std::make_shared<Shader>(shader);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create initial asteroids
    for (int i = 0; i < initialAsteroidCount; i++)
    {
        float spawnDistanceZ = -50.0f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / 35.0f); // [-50.0, -15.0]
        createAsteroid(spawnDistanceZ);
    }
}

void AsteroidSpawner::update(float deltaTime)
{
    timeSinceLastSpawn += deltaTime;

    if (timeSinceLastSpawn >= spawnInterval)
    {
        timeSinceLastSpawn = 0.0f;
        const float spawnDistanceZ = -50.0f;
        createAsteroid(spawnDistanceZ);
    }

    for (auto& asteroid : activeAsteroids)
    {
        asteroid->update(deltaTime);
    }
}

void AsteroidSpawner::createAsteroid(const float spawnDistanceZ)
{
    const float randomOffsetRangeXY = 2.0;

    glm::vec3 startPos;
    glm::vec3 cameraPosition = glm::vec3(0, 0, 0);

    float randX = -randomOffsetRangeXY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2* randomOffsetRangeXY));
    float randY = -randomOffsetRangeXY + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (2 * randomOffsetRangeXY));

    std::cout << "Asteroid randX: " << randX << " randY: " << randY << std::endl;

    startPos.x = cameraPosition.x + randX;
    startPos.y = cameraPosition.y + randY;
    startPos.z = cameraPosition.z + spawnDistanceZ;

    if (model)
    {
        std::shared_ptr<Asteroid> newAsteroid = std::make_shared<Asteroid>();
        glm::vec3 direction = glm::normalize(cameraPosition - startPos);
        newAsteroid->create(*model, *shader, startPos, direction);
        activeAsteroids.push_back(newAsteroid);
        Engine::getInstance().addGameObject(newAsteroid);
    }
    else
    {
        std::cerr << "Nie uda³o siê za³adowaæ modelu asteroidy!" << std::endl;
    }
}

void AsteroidSpawner::render()
{

}