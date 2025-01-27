#include "InGameState.h"
#include "Engine.h"
#include "Player.h"
#include "EnemyUnit.h"
#include "Projectile.h"
#include "AsteroidSpawner.h"
#include "StarSpawner.h"
#include "EnemySpawner.h"
#include "Billboard.h"
#include "ChargingStation.h"

InGameState::InGameState() : GameState(GAME_STATE::IN_GAME) {}

void InGameState::update(float deltaTime)
{
    GameState::update(deltaTime);

    // Get all projectiles
    vector<shared_ptr<Projectile>> enemyProjectiles;
    vector<shared_ptr<Projectile>> playerProjectiles;
    vector<shared_ptr<EnemyUnit>> enemies;

    for (int i = 0; i < gameObjects.size(); ++i)
    {
        auto projectile = dynamic_pointer_cast<Projectile>(gameObjects[i]);
        if (projectile)
        {
            if (projectile->getTeam() == TEAM::ENEMY)
                enemyProjectiles.push_back(projectile);
            else
                playerProjectiles.push_back(projectile);

            continue;
        }
        auto enemy = dynamic_pointer_cast<EnemyUnit>(gameObjects[i]);
        if (enemy)
        {
            enemies.push_back(enemy);
        }
    }

    // Check for collisions with projectiles
    for (const auto& projectile : playerProjectiles)
    {
        for (const auto& enemy : enemies)
        {
            if (enemy->isVertexInsideBbox(projectile->getWorldBboxCenter()))
            {
                enemy->updateHealth(-PROJECTILE_DAMAGE);
                projectile->setAlive(false);
            }
        }
    }

    auto player = this->player.lock();
    if (!player)
        return;

    for (const auto& projectile : enemyProjectiles)
    {
        if (player->isVertexInsideBbox(projectile->getWorldBboxCenter()))
        {
            player->updateHealth(-PROJECTILE_DAMAGE);
            projectile->setAlive(false);
        }
    }
}

void InGameState::render()
{
    GameState::render();
    string killedEnemies = "Points: " + to_string(EnemySpawner::getRemovedEnemiesCount());
    Engine::getInstance().renderText(killedEnemies, glm::vec2(10.0f, SCREEN_HEIGHT - 40.f));
}

void InGameState::onEnter()
{
    auto player = make_shared<Player>();
    this->player = player;

    auto defaultModelShader = Engine::getInstance().GetDefaultModelShader();
    auto defaultEmissiveShader = Engine::getInstance().GetDefaultEmissiveShader();

    shared_ptr<Model> playerModel = Engine::getInstance().getModel(PLAYER_MODEL_PATH);
    shared_ptr<Model> modelProjectile = Engine::getInstance().getModel(PROJECTILE_MODEL_PATH);
    if (playerModel && modelProjectile)
    {
        player->create(*playerModel, *defaultModelShader, *modelProjectile);
        player->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
        gameObjects.push_back(player);
    }

    shared_ptr<AsteroidSpawner> asteroidSpawner = make_shared<AsteroidSpawner>();
    vector<shared_ptr<Model>> asteroidModels = Engine::getInstance().loadAsteroidModels();
    if (!asteroidModels.empty())
    {
        asteroidSpawner->create(asteroidModels, *defaultModelShader);
        gameObjects.push_back(asteroidSpawner);
    }

    shared_ptr<StarSpawner> starSpawner = make_shared<StarSpawner>();
    vector<shared_ptr<Texture>> starTextures = Engine::getInstance().loadStarsTextures();
    if (!starTextures.empty())
    {
        starSpawner->create(starTextures, *defaultEmissiveShader);
        gameObjects.push_back(starSpawner);
    }

    shared_ptr<EnemySpawner> enemySpawner = make_shared<EnemySpawner>();
    vector<shared_ptr<Model>> enemyModels;
    enemyModels.push_back(Engine::getInstance().getModel(ENEMY_MODEL_PATH));
    if (!enemyModels.empty() && modelProjectile)
    {
        enemySpawner->create(enemyModels, *defaultModelShader, *modelProjectile);
        gameObjects.push_back(enemySpawner);
    }

    shared_ptr<ChargingStation> station = make_shared<ChargingStation>();
    vector<shared_ptr<Model>> stationModels;
    stationModels.push_back(Engine::getInstance().getModel(STATION_MODEL_PATH));
    if (!stationModels.empty())
    {
        shared_ptr<Model> selectedModel = stationModels[0];
        station->create(*selectedModel, *defaultModelShader);
        gameObjects.push_back(station);
    }
}
