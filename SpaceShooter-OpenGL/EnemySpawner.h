#pragma once
#include "Spawner.h"
#include "EnemyUnit.h"

class EnemySpawner : public Spawner<EnemyUnit>
{
public:
    void create(const vector<shared_ptr<Model>> models, const Shader& shader, const Model& projectileModel);
    void update(float deltaTime) override;
    void render() override {};

private:
    void spawn() override;
    glm::vec3 getRandomSpawnPosition() const override;

    std::shared_ptr<Model> projectileModel;

    const float spawnMargin = 0.5f;
    const float windowHalfSize = 3.5f;
    const float minEnemyDistance = 1.0f;
};

