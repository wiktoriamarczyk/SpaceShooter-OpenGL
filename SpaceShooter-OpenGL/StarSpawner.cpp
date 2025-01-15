#include "StarSpawner.h"
#include "Engine.h"

void StarSpawner::create(const vector<shared_ptr<Texture>> textures, const Shader& shader)
{
    this->textures = textures;

    maxActiveObjects = 128;
    initialObjectCount = 64;
    maxObjectsPerSpawn = 32;
    spawnInterval = 0.5f;

    Spawner::create(shader);
}

void StarSpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);

    // Erase inactive stars
    for (int i = 0; i < activeObjects.size();)
    {
        if (!activeObjects[i]->isAlive())
        {
            activeObjects.erase(activeObjects.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void StarSpawner::spawn()
{
    int randomTextureIndex = std::rand() % textures.size();
    shared_ptr<Texture> texture = textures[randomTextureIndex];

    if (texture)
    {
        glm::vec3 spawnPosition = getRandomSpawnPosition();
        if (!initialized)
        {
            spawnPosition = getRandomInitialSpawnPosition();
        }

        glm::vec3 cameraPosition = Engine::getInstance().getCameraPosition();
        shared_ptr<Star> newStar = make_shared<Star>();
        float size = randomFloat(0.03f, 0.1f);
        newStar->setSize(glm::vec3(size, size, size));
        newStar->setPosition(spawnPosition);
        newStar->create(*texture, *shader, cameraPosition);
        activeObjects.push_back(newStar);
        Engine::getInstance().addGameObject(newStar);
    }
    else
    {
        std::cerr << "Couldn't load star texture!" << std::endl;
    }
}

glm::vec3 StarSpawner::getRandomSpawnPosition() const
{
    float randZ = randomFloat(-50.0f, -30.0f);

    return glm::vec3(randomCelesticalBodyPosition(-2.f, 2.f), randZ);
}

glm::vec3 StarSpawner::getRandomInitialSpawnPosition() const
{
    float randZ = randomFloat(-20.0f, 0.0f);

    return glm::vec3(randomCelesticalBodyPosition(-2.f, 2.f), randZ);
}
