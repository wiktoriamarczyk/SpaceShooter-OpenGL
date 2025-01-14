#pragma once
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"

template <class T>
class Spawner : public GameObject
{
public:
    void create(const Shader& shader);
    void update(float deltaTime) override;
    void render() override {};

protected:
    virtual void spawn() = 0;
    virtual glm::vec3 getRandomSpawnPosition() const = 0;
    virtual glm::vec3 getRandomInitialSpawnPosition() const { return glm::vec3(0, 0, 0); };
    virtual void spawnInitialObjects();
    void eraseInactiveObjects();

    vector<shared_ptr<T>> activeObjects;
    shared_ptr<Shader> shader;

    float spawnInterval = 2.0f;
    float timeSinceLastSpawn = 0.0f;
    int initialObjectCount = 0;
    int maxObjectsPerSpawn = 1;
    bool initialized = false;
    optional<int> maxActiveObjects = nullopt;
};

template<class T>
void Spawner<T>::create(const Shader& shader)
{
    this->shader = shader.getSelf();

    srand(static_cast<unsigned>(time(nullptr)));

    if (initialObjectCount > 0)
        spawnInitialObjects();
}

template <class T>
void Spawner<T>::update(float deltaTime)
{
    timeSinceLastSpawn += deltaTime;

    // if maxActiveObjects is not set, spawn as many objects as possible
    // otherwise, spawn only if the number of active objects is less than maxActiveObjects
    if (timeSinceLastSpawn >= spawnInterval &&
        (!maxActiveObjects.has_value() || (maxActiveObjects.has_value() && activeObjects.size() < maxActiveObjects)))
    {
        timeSinceLastSpawn = 0.0f;

        int objectsToSpawn = rand() % maxObjectsPerSpawn + 1;

        for (int i = 0; i < objectsToSpawn; i++)
        {
            spawn();
        }
    }

}

template<class T>
void Spawner<T>::eraseInactiveObjects()
{
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

template<class T>
void Spawner<T>::spawnInitialObjects()
{
    for (int i = 0; i < initialObjectCount; i++)
    {
        spawn();
    }
    initialized = true;
}