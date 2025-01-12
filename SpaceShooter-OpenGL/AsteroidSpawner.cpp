#include "AsteroidSpawner.h"
#include "Engine.h"
#include "Shader.h"

void AsteroidSpawner::create(const Model& model, const Shader& shader)
{
    this->model = std::make_shared<Model>(model);  // Przechowywanie modelu
    this->shader = std::make_shared<Shader>(shader);  // Przechowywanie shadera
}

void AsteroidSpawner::update(float deltaTime)
{
    // Aktualizowanie czasu od ostatniego stworzenia asteroidy
    timeSinceLastSpawn += deltaTime;

    // Je�li czas od ostatniego tworzenia asteroid przekroczy� interwa�, tworzymy now� asteroid�
    if (timeSinceLastSpawn >= spawnInterval)
    {
        // Zresetowanie czasu od ostatniego spawnu
        timeSinceLastSpawn = 0.0f;

        // Tworzymy asteroid�, przekazuj�c pozycj� gracza
        glm::vec3 playerPosition = Engine::getInstance().getPlayerPosition();
        createAsteroid(playerPosition);
    }

    // Aktualizujemy wszystkie aktywne asteroidy
    for (auto& asteroid : activeAsteroids)
    {
        asteroid->update(deltaTime);
    }
}

void AsteroidSpawner::createAsteroid(const glm::vec3& playerPosition)
{
    // Zakres okna
    float windowHalfSize = 0.75f; // Po�owa szeroko�ci/wysoko�ci okienka
    float spawnMargin = 0.5f;     // Margines, �eby asteroidy by�y jeszcze dalej poza ekranem

    // Losowanie pozycji startowej asteroidy (poza ekranem)
    glm::vec3 startPos;

    // Losowanie dla osi X
    if (std::rand() % 2 == 0) // 50% szans na lew� lub praw� stron�
        startPos.x = -(windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f); // Po lewej
    else
        startPos.x = windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f;  // Po prawej

    // Losowanie dla osi Y
    if (std::rand() % 2 == 0) // 50% szans na g�r� lub d�
        startPos.y = -(windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f); // Na dole
    else
        startPos.y = windowHalfSize + spawnMargin + static_cast<float>(std::rand() % 100) / 100.0f;   // Na g�rze

    // Z b�dzie sta�e
    startPos.z = 0.0f;

    // Tworzenie nowej asteroidy
    if (model)
    {
        std::shared_ptr<Asteroid> newAsteroid = std::make_shared<Asteroid>();
        newAsteroid->create(*model, *shader, startPos, playerPosition); // U�ywamy metody instancyjnej create
        activeAsteroids.push_back(newAsteroid);  // Dodajemy asteroid� do listy aktywnych asteroid
        Engine::getInstance().addGameObject(newAsteroid);  // Dodajemy do engine
    }
    else
    {
        std::cerr << "Nie uda�o si� za�adowa� modelu asteroidy!" << std::endl;
    }
}

void AsteroidSpawner::render()
{
    // Implementacja renderowania (mo�esz na razie pozostawi� pust�)
}