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

#include<SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <algorithm>
#include <filesystem>
#include <map>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::vector;
using std::to_string;
using std::move;
using std::optional;
using std::nullopt;
using std::srand;
using std::time;
using std::enable_shared_from_this;
using std::min;
using std::max;
using std::function;
using std::map;
using std::pair;
using std::unique_ptr;
using std::make_unique;
using std::unordered_map;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

constexpr const char* WINDOW_NAME = "Space Shooter";

constexpr const char* VS_FILE_NAME = "../Data/shaders/shader.vs";
constexpr const char* FS_FILE_NAME = "../Data/shaders/shader.fs";
constexpr const char* VS_SPRITE_FILE_NAME = "../Data/shaders/sprite_shader.vs";
constexpr const char* FS_SPRITE_FILE_NAME = "../Data/shaders/sprite_shader.fs";
constexpr const char* VS_EMISSIVE_FILE_NAME = "../Data/shaders/emissive_shader.vs";
constexpr const char* FS_EMISSIVE_FILE_NAME = "../Data/shaders/emissive_shader.fs";
constexpr const char* VS_BBOX_FILE_NAME = "../Data/shaders/bbox_shader.vs";
constexpr const char* FS_BBOX_FILE_NAME = "../Data/shaders/bbox_shader.fs";
constexpr const char* VS_TEXT_FILE_NAME = "../Data/shaders/text_shader.vs";
constexpr const char* FS_TEXT_FILE_NAME = "../Data/shaders/text_shader.fs";

constexpr const char* BACKGROUND_PATH = "../Data/Textures/background.png";
constexpr const char* PLAYER_MODEL_PATH = "../Data/Models/space_ship_wg-02/scene.gltf";
constexpr const char* ENEMY_MODEL_PATH = "../Data/Models/empire_space_ship/scene.gltf";
constexpr const char* PROJECTILE_MODEL_PATH = "../Data/Models/rusty_metal_sphere/scene.gltf";
constexpr const char* STATION_MODEL_PATH = "../Data/Models/space_ship_-_lost_in_space/scene.gltf";
constexpr const char* CURSOR_PATH = "../Data/Textures/cursor.png";
constexpr const char* MUSIC_PATH = "../Data/Audio/Stealer.ogg";
constexpr const char* SHOOT_SOUND_PATH = "../Data/Audio/laser.mp3";
constexpr const char* EXPLOSION_SOUND_PATH = "../Data/Audio/explosion.mp3";

constexpr bool WIREFRAME_MODE = false;
constexpr bool SHOW_BOUNDING_BOX = false;

const int PROJECTILE_DAMAGE = 5;
constexpr glm::vec3 WHITE_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

enum class TEAM
{
    PLAYER,
    ENEMY
};

enum class GAME_STATE
{
    MENU,
    IN_GAME,
    GAME_OVER,
    UNKNOWN
};

struct Character
{
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

static float randomFloat(float min, float max)
{
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

// so ugly ;_; but i don't have time so i'll leave it like this for now (forever)
static glm::vec2 randomCelesticalBodyPosition(float min, float max)
{
    float x = 0;
    float y = 0;
    do
    {
        x = randomFloat(min, max);
        y = randomFloat(min, max);
    } while (x > -1.0f && x < 1.0f && y > -1.0f && y < 1.0f);

    return glm::vec2(x, y);
}

static float angleDifference(float angle1, float angle2)
{
    // constraint the angles to be in the range [0, 360]
    float diff = fmod(angle2 - angle1, 360.0);
    if (diff > 180.0f)
        diff -= 360.0f;
    else if (diff < -180.0f)
        diff += 360.0f;

    return diff;
}

static unsigned char* loadImage(const char* filepath, int* width, int* height, int* channels)
{
    unsigned char* data = stbi_load(filepath, width, height, channels, 4);
    if (!data)
    {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return nullptr;
    }
    return data;
}

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