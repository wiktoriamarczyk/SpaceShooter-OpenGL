#include "Star.h"
#include "Shader.h"
#include "Engine.h"

void Star::create(int index, const Texture& texture, const Shader& shader, glm::vec3 cameraPosition)
{
    shaderArrayIndex = index;
    modelShader = Engine::GetDefaultModelShader();
    offScreenDeathEnabled = true;
    name = "Star";

    Billboard::create(texture, shader, cameraPosition);

    updateShaderData();
}

void Star::update(float deltaTime)
{
    // Move star towards camera
    position.z += speed * deltaTime;

    // Update light position in model shader
    string uniformName = "pointLights[" + to_string(shaderArrayIndex) + "]";
    modelShader->use();
    modelShader->setVec3(uniformName + ".position", position);

    Billboard::update(deltaTime);
}

void Star::updateShaderData()
{
    string uniformName = "pointLights[" + to_string(shaderArrayIndex) + "]";

    // Update light position in model shader
    modelShader->use();
    modelShader->setVec3(uniformName + ".position", position);

    // Update light parameters in star's shader
    modelShader->setVec3(uniformName + ".diffuse", glm::vec3(0.55164f, 0.40648f, 0.02648f));
    modelShader->setVec3(uniformName + ".specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
    modelShader->setFloat(uniformName + ".constant", 1.0f);
    modelShader->setFloat(uniformName + ".linear", 0.7f);
    modelShader->setFloat(uniformName + ".quadratic", 1.8f);
}

void Star::setIndex(int index)
{
    shaderArrayIndex = index;

    // Update light position in model shader
    string uniformName = "pointLights[" + to_string(shaderArrayIndex) + "]";
    updateShaderData();
}

void Star::render()
{
    int blendDefaultFlag1 = GL_SRC_ALPHA;
    int blendDefaultFlag2 = GL_ONE_MINUS_SRC_ALPHA;

    // enable blending
    glEnable(GL_BLEND);
    // set additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    Billboard::render();

    // reset blending
    glBlendFunc(blendDefaultFlag1, blendDefaultFlag2);
}
