#include "Shader.h"

bool Shader::initialize(const char* vertexPath, const char* fragmentPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;

    unsigned int vertexShader = initVertexShader();
    unsigned int fragmentShader = initFragmentShader();

    if (vertexShader == -1 || fragmentShader == -1)
    {
        return false;
    }

    ID = linkShaders(vertexShader, fragmentShader);

    if (ID == -1)
    {
        return false;
    }

    return true;
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    auto loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    auto loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

unsigned int Shader::initVertexShader()
{
    string vertexShaderSource;
    if (!readShaderSource(vertexPath.c_str(), vertexShaderSource))
    {
        std::cout << "Failed to read vertex shader source" << std::endl;
        return -1;
    }

    const char* vertexShaderSourceArray[1] = { vertexShaderSource.c_str() };
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, vertexShaderSourceArray, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    return vertexShader;
}

unsigned int Shader::initFragmentShader()
{
    string fragmentShaderSource;
    if (!readShaderSource(fragmentPath.c_str(), fragmentShaderSource))
    {
        std::cout << "Failed to read fragment shader source" << std::endl;
        return -1;
    }

    const char* fragmentShaderSourceArray[1] = { fragmentShaderSource.c_str() };
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, fragmentShaderSourceArray, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    int success;
    char infoLog[512];

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    return fragmentShader;
}

unsigned int Shader::linkShaders(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

bool Shader::readShaderSource(const char* filename, string& source)
{
    FILE* file;
    fopen_s(&file, filename, "r");

    if (!file)
    {
        return false;
    }

    size_t fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    source.resize(fileSize);
    fread(&source[0], 1, fileSize, file);
    fclose(file);

    return true;
}
