#pragma once
#include "GameObject.h"
#include "EnemyUnit.h"
#include "Model.h"
#include "Shader.h"
class EnemySpawner : public GameObject
{
public:
    void create(const Model& enemyModel, const Shader& enemyShader, const Model& projectileModel);
    void update(float deltaTime);
    void render() override;

private:
    void spawnEnemy(); 
    glm::vec3 getRandomSpawnPosition() const; 

    std::vector<std::shared_ptr<EnemyUnit>> activeEnemies; 
    std::shared_ptr<Model> enemyModel; 
    std::shared_ptr<Shader> enemyShader; 
    std::shared_ptr<Model> projectileModel; 

    float spawnInterval = 3.0f; 
    float timeSinceLastSpawn = 0.0f; 

    const float spawnMargin = 0.5f; 
    const float windowHalfSize = 3.5f; 
    const size_t maxEnemies = 3; // Maksymalna liczba przeciwników w grze
    const float minEnemyDistance = 1.0f;
};

