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

    for (int i = 0; i < activeObjects.size();)
    {
        if (!activeObjects[i]->isAlive())
        {
            int shaderIndex = activeObjects[i]->getIndex();
            activeObjects.erase(activeObjects.begin() + i);

            updatePointLightsCount();

            // Move indices of objects that are rendered after the deleted object
            for (int j = shaderIndex; j < activeObjects.size(); ++j)
            {
                activeObjects[j]->setIndex(j);
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

        float size = randomFloat(0.03f, 0.1f);
        newStar->setSize(glm::vec3(size, size, size));
        newStar->setPosition(spawnPosition);

        int starIndex = activeObjects.size();
        newStar->create(starIndex, *texture, *shader, cameraPosition);

        activeObjects.push_back(newStar);
        Engine::getInstance().addGameObject(newStar);

        updatePointLightsCount();
    }
    else
    {
        std::cerr << "Nie uda³o siê za³adowaæ modelu asteroidy!" << std::endl;
    }


    std::cout << "Active objects: " << activeObjects.size() << std::endl;
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

void StarSpawner::updatePointLightsCount()
{
    // Update shader lights count in model shader
    auto modelShader = Engine::GetDefaultModelShader();
    modelShader->use();
    modelShader->setInt("pointLightsCount", activeObjects.size());
}
