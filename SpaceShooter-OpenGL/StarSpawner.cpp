#include "StarSpawner.h"
#include "Engine.h"

void StarSpawner::create(const vector<shared_ptr<Texture>> textures, const Shader& shader)
{
    this->textures = textures;

    initialObjectCount = 30;
    maxObjectsPerSpawn = 15;
    spawnInterval = 0.02f;

    Spawner::create(shader);
}

void StarSpawner::update(float deltaTime)
{
    Spawner::update(deltaTime);

    for (int i = 0; i < activeObjects.size();)
    {
        if (!activeObjects[i]->isAlive())
        {
            int shaderIndex = activeObjects[i]->getIndex();
            activeObjects.erase(activeObjects.begin() + i);

            // Move indices of objects that are rendered after the deleted object
            for (int j = shaderIndex; j < activeObjects.size(); ++j)
            {
                activeObjects[j]->setIndex(j);
                activeObjects[j]->updateShaderData();
            }
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

        std::shared_ptr<Star> newStar = std::make_shared<Star>();
        newStar->setSize(glm::vec3(0.5f, 0.5f, 0.5f));
        newStar->setPosition(spawnPosition);

        int starIndex = activeObjects.size();
        newStar->create(starIndex, *texture, *shader, cameraPosition);

        activeObjects.push_back(newStar);
        Engine::getInstance().addGameObject(newStar);

        // Update shader data
        shader->use();
        shader->setInt("pointLightsCount", activeObjects.size());
    }
    else
    {
        std::cerr << "Nie uda³o siê za³adowaæ modelu asteroidy!" << std::endl;
    }
}

glm::vec3 StarSpawner::getRandomSpawnPosition() const
{
    float randX = randomFloat(-1.f, 1.f);
    float randY = randomFloat(-1.f, 1.f);
    float randZ = randomFloat(-50.0f, -30.0f);

    return glm::vec3(randX, randY, randZ);
}

glm::vec3 StarSpawner::getRandomInitialSpawnPosition() const
{
    float randX = randomFloat(-1.f, 1.f);
    float randY = randomFloat(-1.f, 1.f);
    float randZ = randomFloat(-20.0f, 0.0f);

    return glm::vec3(randX, randY, randZ);
}
