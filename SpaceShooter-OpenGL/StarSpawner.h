#pragma once
#include "Spawner.h"
#include "Star.h"

class StarSpawner : public Spawner<Star>
{
public:
    void create(const vector<shared_ptr<Texture>> textures, const Shader& shader);
    void update(float deltaTime) override;
    void render() override {};

protected:
    void spawn() override;
    glm::vec3 getRandomSpawnPosition() const override;
    glm::vec3 getRandomInitialSpawnPosition() const override;

    vector<shared_ptr<Texture>> textures;

private:
    void updateShaderData(int index);
};

