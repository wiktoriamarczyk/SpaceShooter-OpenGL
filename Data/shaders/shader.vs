#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec3 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // transform vertex position to world-space by multiplying it with the model matrix
    FragPos = vec3(model * vec4(aPos, 1.0));
    Color = aColor;
    TexCoord = aTexCoord;
    // transform normal vector to world-space by multiplying it with the normal matrix
    // normal matrix - the transpose of the inverse of the upper-left 3x3 part of the model matrix
    Normal = normalMatrix * aNormal;
}