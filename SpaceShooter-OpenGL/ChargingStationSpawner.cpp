#include "ChargingStationSpawner.h"

void ChargingStationSpawner::create(const vector<shared_ptr<Model>> models, const Shader& shader)
{
    this->models = models;

    spawnInterval = randomFloat(60, 150);

    Spawner::create(shader);
}

void ChargingStationSpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);
    eraseInactiveObjects();
}

void ChargingStationSpawner::spawn()
{
    int randomModelIndex = std::rand() % models.size();
    shared_ptr<Model> selectedModel = models[randomModelIndex];

    if (!selectedModel)
    {
        std::cerr << "Couldn't load charging station model!" << std::endl;
        return;
    }

    shared_ptr<ChargingStation> station = std::make_shared<ChargingStation>();
    station->create(*selectedModel, *shader);

    activeObjects.push_back(station);
    Engine::getInstance().addGameObject(station);

    spawnInterval = randomFloat(60, 150);
}
