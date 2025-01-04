#pragma once

#include "Common.h"

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // reads and builds the shader
    bool load(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int initVertexShader();
    unsigned int initFragmentShader();
    unsigned int linkShaders(unsigned int vertexShader, unsigned int fragmentShader);
    bool readShaderSource(const char* filename, string& source);
};
