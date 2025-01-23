#include "HealthBar.h"
#include "Engine.h"
#include "VertexBuffer.h"
#include <glm/gtc/matrix_transform.hpp>

HealthBar::HealthBar(glm::vec2 position, glm::vec2 size, float maxHealth)
    : position(position), size(size), maxHealth(maxHealth), currentHealth(maxHealth)
{
    // Kolor t³a paska zdrowia
    backgroundColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    // Kolor zdrowia
    healthColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    VAO = Engine::GetDefaultVAO();
    VBO = Engine::GetDefaultVBO();
    shader = Engine::GetDefaultSpriteShader();
}

// Ustawia aktualny poziom zdrowia
void HealthBar::setHealth(float health) {
    currentHealth = glm::clamp(health, 0.0f, maxHealth);
}

// Rysuje prostok¹t
void HealthBar::drawRectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) const {
    if (!shader || !VAO)
        return;

    shader->use();
    VAO->bind();

    // Macierz transformacji
    glm::mat4 model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader->setMat4("model", model);
    shader->setVec4("color", color); // Przekazanie koloru do shadera

    glDrawArrays(GL_TRIANGLES, 0, VBO->getCount());
}

// Rysuje ca³y pasek zdrowia
void HealthBar::draw() const {

    drawRectangle(position, size, backgroundColor);

    float healthPercentage = glm::clamp(currentHealth / maxHealth, 0.0f, 1.0f);
    glm::vec2 healthBarSize = glm::vec2(size.x * healthPercentage, size.y);

    drawRectangle(position, healthBarSize, healthColor);
}

void HealthBar::update(float deltaTime) {
    
}

void HealthBar::render() {
    draw();
}