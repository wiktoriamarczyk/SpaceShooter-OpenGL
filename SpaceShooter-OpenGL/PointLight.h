#pragma once
#include "Common.h"
#include "Shader.h"

class PointLight
{
public:
    ~PointLight();
    void create(int index);
    void setParameters(glm::vec3 diffuse, glm::vec3 specular, float linear, float quadratic);
    void updateLightParameters();
    void updateLightPosition(glm::vec3 position);
    int getIndex() const { return shaderArrayIndex; }
    void setNewIndex(int index);
    void setAdditiveBlending(bool value);
    shared_ptr<Shader> getModelShader() const { return modelShader; }

protected:
    int shaderArrayIndex = -1;
    shared_ptr<Shader> lightShader;
    shared_ptr<Shader> modelShader;

    glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
    float constant = 1.f;
    float linear = 0.7f;
    float quadratic = 1.8f;

    string getUniformName() const { return "pointLights[" + to_string(shaderArrayIndex) + "]"; }
};

