#pragma once
#include "Spawner.h"
#include "ChargingStation.h"
#include "Shader.h"

class ChargingStationSpawner : public Spawner<ChargingStation>
{
public:
    void create(const vector<shared_ptr<Model>> models, const Shader& shader);
    void update(float deltaTime);
    void render() override {};
protected:
    vector<shared_ptr<Model>> models;
    void spawn() override;
};

