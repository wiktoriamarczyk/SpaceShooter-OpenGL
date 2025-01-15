#include "ChargingStationSpawner.h"

void ChargingStationSpawner::create(const vector<shared_ptr<Model>> models, const Shader& shader)
{
    this->models = models;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    spawnInterval = static_cast<float>((std::rand() % 91) + 60);

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

    shared_ptr<ChargingStation> station = std::make_shared<ChargingStation>();
    station->create(*selectedModel, *shader);

    activeObjects.push_back(station);
    Engine::getInstance().addGameObject(station);

    spawnInterval = static_cast<float>((std::rand() % 91) + 60);
}
