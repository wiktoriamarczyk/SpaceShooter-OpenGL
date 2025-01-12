#include "AsteroidSpawner.h"
#include "Engine.h"
#include "Shader.h"

void AsteroidSpawner::create(const Model& model, const Shader& shader)
{
    this->model = std::make_shared<Model>(model);  
    this->shader = std::make_shared<Shader>(shader);  

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void AsteroidSpawner::update(float deltaTime)
{
    timeSinceLastSpawn += deltaTime;

    if (timeSinceLastSpawn >= spawnInterval)
    {
        timeSinceLastSpawn = 0.0f;

        glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();
        createAsteroid(playerPosition);
    }

    for (auto& asteroid : activeAsteroids)
    {
        asteroid->update(deltaTime);
    }
}

void AsteroidSpawner::createAsteroid(const glm::vec3& playerPosition)
{
    float windowHalfSize = 0.75f; 
    float spawnMargin = 0.5f;    

    glm::vec3 startPos;

    if (std::rand() % 2 == 0) 
        startPos.x = -(windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f); 
    else
        startPos.x = windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f; 

    if (std::rand() % 2 == 0) 
        startPos.y = -(windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f); 
    else
        startPos.y = windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f;  

    startPos.z = 0.0f;

    if (model)
    {
        std::shared_ptr<Asteroid> newAsteroid = std::make_shared<Asteroid>();
        newAsteroid->create(*model, *shader, startPos, playerPosition); 
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