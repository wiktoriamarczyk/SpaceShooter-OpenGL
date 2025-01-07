#include "EnemyUnit.h"

void EnemyUnit::create(const Model& model, const Shader& shader)
{
    ModelObject::create(model, shader);
    rotation.x = 0.f;
    rotation.y = 0.f;
    position.y = 1.0f;

    movementDirection = glm::vec2(1.0f, 0.0f); 
    speed = 2.0f;  
    minX = -1.5f;  
    maxX = 1.5f;   
    idleTime = 0.0f; 

    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void EnemyUnit::update(float deltaTime)
{
    if (idleTime > 0.0f)
    {
        idleTime -= deltaTime;
        if (idleTime <= 0.0f)
        {
            
            speed = static_cast<float>(std::rand() % 3 + 1); 

            movementDirection.x = (std::rand() % 2 == 0) ? 1.0f : -1.0f;

            float newTargetX;
            do {
                newTargetX = static_cast<float>(std::rand() % 100) / 100.0f * (maxX - minX) + minX;
            } while (std::abs(newTargetX - position.x) < minDistance); 

            targetX = newTargetX;
        }
        return; 
    }

    position += glm::vec3(movementDirection.x * speed * deltaTime, 0.f, 0.f);

    if ((movementDirection.x > 0 && position.x >= targetX) || (movementDirection.x < 0 && position.x <= targetX))
    {
        setRandomIdleTime();
    }

    ModelObject::update(deltaTime);
}

void EnemyUnit::render()
{
    ModelObject::render();
}

void EnemyUnit::setRandomIdleTime()
{
    idleTime = static_cast<float>(std::rand() % 3 + 1); 
}