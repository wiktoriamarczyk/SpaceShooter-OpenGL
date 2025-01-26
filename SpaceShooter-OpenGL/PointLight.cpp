#include "PointLight.h"
#include "Engine.h"

PointLight::~PointLight()
{
    Engine::getInstance().removePointLight(*this);
}

void PointLight::create(int index)
{
    shaderArrayIndex = index;
    modelShader = Engine::GetDefaultModelShader();

    updateLightParameters();
}

void PointLight::setParameters(glm::vec3 diffuse, glm::vec3 specular, float linear, float quadratic)
{
    this->diffuse = diffuse;
    this->specular = specular;
    this->linear = linear;
    this->quadratic = quadratic;

    updateLightParameters();
}

void PointLight::updateLightParameters()
{
    string uniformName = getUniformName();

    modelShader->use();
    modelShader->setVec3(uniformName + ".diffuse", diffuse);
    modelShader->setVec3(uniformName + ".specular", specular);
    modelShader->setFloat(uniformName + ".constant", constant);
    modelShader->setFloat(uniformName + ".linear", linear);
    modelShader->setFloat(uniformName + ".quadratic", quadratic);
}

void PointLight::updateLightPosition(glm::vec3 position)
{
    string uniformName = getUniformName();

    modelShader->use();
    modelShader->setVec3(uniformName + ".position", position);
}

void PointLight::setNewIndex(int index)
{
    shaderArrayIndex = index;
    updateLightParameters();
}

void PointLight::setAdditiveBlending(bool value)
{
    int blendDefaultFlag1 = GL_SRC_ALPHA;
    int blendDefaultFlag2 = GL_ONE_MINUS_SRC_ALPHA;

    // enable blending
    glEnable(GL_BLEND);

    if (value)
    {
        // set additive blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        // reset blending
        glBlendFunc(blendDefaultFlag1, blendDefaultFlag2);
    }
}
