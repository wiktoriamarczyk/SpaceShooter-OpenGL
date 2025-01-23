#pragma once
#include "Common.h"
#include "Shader.h"
#include "VertexArrayObject.h"
#include "GameObject.h"
#include <glm/glm.hpp>

class HealthBar : public GameObject
{
public:
    HealthBar(glm::vec2 position = glm::vec2(SCREEN_WIDTH / 2 + 285.0f, SCREEN_HEIGHT - 27.0f),
        glm::vec2 size = glm::vec2(200.0f, 20.0f), float maxHealth = 100.0f);

    void setHealth(float health);

    void update(float deltaTime) override; 
    void render() override;              

private:
    glm::vec2 position;           
    glm::vec2 size;          
    float maxHealth;             
    float currentHealth;     
    glm::vec4 backgroundColor;   
    glm::vec4 healthColor;        

    shared_ptr<VertexArrayObject> VAO;
    shared_ptr<VertexBuffer> VBO;
    shared_ptr<Shader> shader;

    void drawRectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) const;

    void draw() const;
};

