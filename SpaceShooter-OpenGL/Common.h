#pragma once

#include "stb_image.h"
\
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;
using std::vector;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

constexpr const char* WINDOW_NAME = "Space Shooter";

const string VS_FILE_NAME = "shader.vs";
const string FS_FILE_NAME = "shader.fs";

constexpr bool WIREFRAME_MODE = false;