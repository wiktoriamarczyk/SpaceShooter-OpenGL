#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Asteroid.h"

class AsteroidSpawner : public GameObject
{
public:
    void create(const Model& model, const Shader& shader);
    void update(float deltaTime) override;
    void createAsteroid(const float spawnDistanceZ);
    void render() override;

private:
    std::vector<std::shared_ptr<Asteroid>> activeAsteroids;
    float spawnInterval = 5.0f;
    float timeSinceLastSpawn;
    shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;
    int initialAsteroidCount = 5;
};

