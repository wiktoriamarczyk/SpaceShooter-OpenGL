#pragma once

#include "stb_image.h"
\
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::vector;
using std::to_string;
using std::move;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

constexpr const char* WINDOW_NAME = "Space Shooter";

const string VS_FILE_NAME = "../Data/Shaders/shader.vs";
const string FS_FILE_NAME = "../Data/Shaders/shader.fs";
constexpr const char* PLAYER_MODEL_PATH = "../Data/Models/Camo Stellar Jet/Package/CamoStellarJet.obj";
constexpr const char* ENEMY_MODEL_PATH = "../Data/Models/Meteor Slice/Package/MeteorSlicer.obj";
constexpr const char* PROJECTILE_MODEL_PATH = "../Data/Models/Cube/Cube.obj";
constexpr const char* ASTEROID_MODEL_PATH = "../Data/Models/Sphere/Sphere.obj";

constexpr bool WIREFRAME_MODE = false;

template<typename T>
class SelfHelper
{
public:
    template< typename... Args >
    static shared_ptr<T> create(Args&&... args)
    {
        shared_ptr<T> object;

        if constexpr (requires(T& object) { object.initialize(std::forward<Args>(args)...); })
        {
            object.reset(new T());
            if( !object->initialize(std::forward<Args>(args)...) )
                return nullptr;
        }
        else
        {
            object.reset(new T(std::forward<Args>(args)...));
        }

        object->initSelf(object);
        return object;
    }
    shared_ptr<T> getSelf() const { return self.lock(); }

protected:
    bool initSelf(const shared_ptr<T>& self)
    {
        if (!self || self.get() != this)
            return false;

        this->self = self;
        return true;
    }

private:
    weak_ptr<T> self;
};