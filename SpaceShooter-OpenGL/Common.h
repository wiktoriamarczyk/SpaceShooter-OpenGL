#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

using std::string;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

const char* WINDOW_NAME = "Space Shooter";

const string VS_FILE_NAME = "shader.vs";
const string FS_FILE_NAME = "shader.fs";

bool WIREFRAME_MODE = false;