#include "EnemyUnit.h"
#include "Shader.h"
#include "Engine.h"

void EnemyUnit::create(const Model& model, const Shader& shader, const Model& projectileModel)
{
    ModelObject::create(model, shader);
    this->projectileModel = projectileModel.getSelf();

    rotation.x = 0.f;
    rotation.y = 0.f;
    position = glm::vec3(0.0f, 1.0f, -2.0f);
    shootCooldown = shootInterval;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    remainingShots = 0;
    setRandomIdleTime();
    postShotCooldown = 0.0f;
}

void EnemyUnit::update(float deltaTime)
{
    glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();

    for (int i = 0; i < projectiles.size();)
    {
        if (!projectiles[i]->isAlive())
            projectiles.erase(projectiles.begin() + i);
        else
            i++;
    }

    // Je�li przeciwnik odpoczywa (idle)
    if (idleTime > 0.0f)
    {
        idleTime -= deltaTime;

        if (idleTime <= 0.0f) // Po zako�czeniu pauzy
        {
            setRandomTargetPosition();  // Losowanie celu
            remainingShots = std::rand() % 10 + 1; // Losowa liczba strza��w (od 1 do 10)
            shootCooldown = shootInterval; // Resetujemy cooldown strza��w
        }

        // Strzelanie tylko podczas odpoczynku
        if (remainingShots > 0)
        {
            shootCooldown -= deltaTime;

            if (shootCooldown <= 0.0f) // Kiedy cooldown osi�gnie zero, oddajemy strza�
            {
                shootProjectile(playerPosition); // Strzelanie do gracza
                shootCooldown = shootInterval; // Resetujemy cooldown
                remainingShots--; // Zmniejszamy liczb� pozosta�ych strza��w
            }
        }

        return; // Je�li przeciwnik odpoczywa, nie rusza si�
    }

    // Je�li przeciwnik nie odpoczywa, sprawdzamy czas oczekiwania po strza�ach
    if (remainingShots == 0 && postShotCooldown > 0.0f)
    {
        postShotCooldown -= deltaTime;

        if (postShotCooldown <= 0.0f)
        {
            // Po up�ywie czasu oczekiwania, przeciwnik zacznie si� rusza�
            setRandomTargetPosition(); // Losowanie celu po oczekiwaniu
            currentSpeed = 0.0f;  // Startujemy od zerowej pr�dko�ci
        }

        return; // Je�li czas oczekiwania nie min��, nie rusza si�
    }

    // Je�li pozosta�y strza�y, to strzelamy
    if (remainingShots > 0)
    {
        shootCooldown -= deltaTime;

        if (shootCooldown <= 0.0f) // Kiedy cooldown osi�gnie zero, oddajemy strza�
        {
            shootProjectile(playerPosition); // Strzelanie do gracza
            shootCooldown = shootInterval; // Resetujemy cooldown
            remainingShots--; // Zmniejszamy liczb� pozosta�ych strza��w
        }
    }
    else
    {
        // Je�li strza�y zosta�y zako�czone, wprowadzamy czas oczekiwania przed ruszeniem si�
        if (postShotCooldown <= 0.0f)
        {
            // Je�li czas oczekiwania min��, przeciwnik rusza si� w kierunku celu
            glm::vec3 direction = glm::normalize(targetPosition - position); // Kierunek ruchu

            // Przyspieszanie na pocz�tku ruchu (pr�dko�� ro�nie od 0 do docelowej)
            if (currentSpeed < speed)
            {
                currentSpeed += acceleration * deltaTime; // Przyspieszanie
                currentSpeed = glm::min(currentSpeed, speed); // Osi�gamy docelow� pr�dko��
            }

            // Zwalnianie na ko�cu ruchu (gdy przeciwnik jest blisko celu)
            float distanceToTarget = glm::distance(position, targetPosition);
            if (distanceToTarget < decelerationDistance)
            {
                currentSpeed = glm::max(currentSpeed * (distanceToTarget / decelerationDistance), 0.5f);
            }

            position += direction * currentSpeed * deltaTime;

            // Je�li przeciwnik dotrze do celu, przechodzi w tryb odpoczynku (idle)
            if (distanceToTarget < 0.05f)
            {
                setRandomIdleTime(); // Zatrzymanie na chwil�
            }

            // Ograniczenie pozycji do granic [-1, 1] w osiach X i Y
            position.x = glm::clamp(position.x, minX, maxX);
            position.y = glm::clamp(position.y, minY, maxY);
        }
        else
        {
            // Je�li nadal czeka, nie rusza si�
            postShotCooldown -= deltaTime;
        }
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
    postShotCooldown = 1.0f;
}

void EnemyUnit::setRandomTargetPosition()
{
    float newTargetX, newTargetY;

    do {
        newTargetX = static_cast<float>(std::rand() % 200 - 100) / 100.0f; // Losowy cel w X
        newTargetY = static_cast<float>(std::rand() % 200 - 100) / 100.0f; // Losowy cel w Y
    } while (glm::distance(glm::vec3(newTargetX, newTargetY, 0.0f), position) < minDistance);

    targetPosition = glm::vec3(newTargetX, newTargetY, position.z); // Z zachowaniem sta�ej pozycji Z
}

void EnemyUnit::shootProjectile(const glm::vec3& playerPosition)
{
    if (shader && projectileModel) {
        std::shared_ptr<Projectile> newProjectile = std::make_shared<Projectile>();

        glm::vec3 projectileStartPos = position + glm::vec3(0.0f, -0.1f, 0.0f); // Startowa pozycja pocisku

        // Wyznacz kierunek ruchu pocisku w stron� gracza
        glm::vec3 projectileDirection = glm::normalize(playerPosition - projectileStartPos);

        newProjectile->create(projectileStartPos, projectileDirection, 3.0f, *projectileModel, *shader);
        newProjectile->setSize(glm::vec3(0.01f, 0.03f, 0.01f));
        projectiles.push_back(newProjectile);
        Engine::getInstance().addGameObject(newProjectile);
    }
    else {
        std::cerr << "Shader or model was destroyed or never initialized!" << std::endl;
    }
}