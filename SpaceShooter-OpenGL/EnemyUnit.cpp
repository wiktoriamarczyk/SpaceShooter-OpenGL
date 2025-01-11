#include "EnemyUnit.h"
#include "Shader.h"

void EnemyUnit::create(const Model& model, const Shader& shader, const Model& model2)
{
    ModelObject::create(model, shader);
    //this->shader;
    //this->model2 = model2.getSelf();
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

    // Aktualizacja cooldownu strza³u
    //shootCooldown -= deltaTime;
    //if (shootCooldown <= 0.0f)
    //{
    //    shootProjectile(); // Strzelanie pociskiem
    //    shootCooldown = shootInterval; // Reset cooldownu
    //}

    //// Aktualizacja pocisków
    //for (auto it = projectiles->begin(); it != projectiles.end(); )
    //{
    //    it->update(deltaTime);
    //    if (it->isOffScreen())
    //    {
    //        it = projectiles.erase(it); // Usuwanie pocisku, gdy wyjdzie poza ekran
    //    }
    //    else
    //    {
    //        ++it;
    //    }
    //}

   /* for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i]->update(deltaTime);
    }*/

    ModelObject::update(deltaTime);
}

void EnemyUnit::render() 
{

    ModelObject::render();
    //for (auto& projectile : projectiles)
    //{
    //    projectile->render();
    //}
}

void EnemyUnit::setRandomIdleTime()
{
    idleTime = static_cast<float>(std::rand() % 3 + 1); 
}

//void EnemyUnit::shootProjectile()
//{
//
//    if (shader) {
//        // Tworzenie nowego pocisku
//        shared_ptr<Projectile> newProjectile;
//        glm::vec3 projectileStartPos = position + glm::vec3(0.0f, -0.1f, 0.0f); // Pocz¹tkowa pozycja pocisku
//        glm::vec3 projectileDirection = glm::vec3(0.0f, -1.0f, 0.0f);           // Kierunek - w dó³
//
//        newProjectile->create(projectileStartPos, projectileDirection, 1.0f, *model2, *shader);
//        projectiles.push_back(newProjectile);
//    }
//    else {
//        std::cerr << "Shader was destroyed or never initialized!" << std::endl;
//    }
//}