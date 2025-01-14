#pragma once
#include "Spawner.h"
#include "Asteroid.h"

class AsteroidSpawner : public Spawner<Asteroid>
{
public:
    void create(const vector<shared_ptr<Model>> models, const Shader& shader);
    void update(float deltaTime) override;
    void render() override {};

protected:
    void spawn() override;
    glm::vec3 getRandomSpawnPosition() const override;
    glm::vec3 getRandomInitialSpawnPosition() const override;
};

