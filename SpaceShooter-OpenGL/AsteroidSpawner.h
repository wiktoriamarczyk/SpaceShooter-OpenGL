#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Asteroid.h"

class AsteroidSpawner : public GameObject
{
public:
    void create(const Model& model, const Shader& shader); // Interwa� czasowy mi�dzy tworzeniem asteroid
    void update(float deltaTime) override; // Aktualizacja spawnera i generowanie nowych asteroid
    void createAsteroid(const glm::vec3& playerPosition); // Tworzenie nowej asteroidy
    void render() override;

private:
    std::vector<std::shared_ptr<Asteroid>> activeAsteroids; // Lista aktywnych asteroid
    float spawnInterval = 5.0f; // Czas mi�dzy kolejnymi tworzeniami asteroid
    float timeSinceLastSpawn;
    shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;
};

