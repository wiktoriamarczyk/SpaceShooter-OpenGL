#pragma once
#include "GameObject.h"
#include "Model.h"

class ModelObject : public GameObject
{
public:
    virtual void create(const Model& model, const Shader& shader);
    void update(float deltaTime) override;
    void render() override;
    void onKeyDown(int key) override;
    void onKeyUp(int key) override;

protected:
    shared_ptr<Model> model;
    shared_ptr<Shader> shader;
};

