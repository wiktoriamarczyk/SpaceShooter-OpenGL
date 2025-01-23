#pragma once

#include "Common.h"

class Shader : public SelfHelper<Shader>
{
    friend class SelfHelper<Shader>;
public:
    // the program ID
    unsigned int ID;
    static shared_ptr<Shader> create(const char* vertexPath, const char* fragmentPath)
    {
        return SelfHelper<Shader>::create(vertexPath, fragmentPath);
    }
    string getVertexPath() const { return vertexPath; }
    string getFragmentPath() const { return fragmentPath; }
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;

private:
    string vertexPath;
    string fragmentPath;

    Shader() = default;
    bool initialize(const char* vertexPath, const char* fragmentPath);
    unsigned int initVertexShader();
    unsigned int initFragmentShader();
    unsigned int linkShaders(unsigned int vertexShader, unsigned int fragmentShader);
    bool readShaderSource(const char* filename, string& source);
};
