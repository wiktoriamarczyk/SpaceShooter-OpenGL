#include "Star.h"
#include "Shader.h"

void Star::create(int index, const Texture& texture, const Shader& shader, glm::vec3 cameraPosition)
{
    shaderArrayIndex = index;
    Billboard::create(texture, shader, cameraPosition);
}

void Star::update(float deltaTime)
{
    position.z += speed * deltaTime;
    string uniformName = "pointLights[" + to_string(shaderArrayIndex) + "]";
    shader->use();
    shader->setVec3(uniformName + ".position", position);

    Billboard::update(deltaTime);
}

void Star::updateShaderData()
{
    string uniformName = "pointLights[" + to_string(shaderArrayIndex) + "]";

    shader->use();
    shader->setVec3(uniformName + ".position", position);
    shader->setVec3(uniformName + ".ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
    shader->setVec3(uniformName + ".diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
    shader->setVec3(uniformName + ".specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
    shader->setFloat(uniformName + ".constant", 1.0f);
    shader->setFloat(uniformName + ".linear", 0.09f);
    shader->setFloat(uniformName + ".quadratic", 0.032f);
}

void Star::setIndex(int index)
{
    shaderArrayIndex = index;
    string uniformName = "pointLights[" + to_string(shaderArrayIndex) + "]";
    shader->use();
    shader->setVec3(uniformName + ".position", position);
}

void Star::render()
{
    Billboard::render();
}
