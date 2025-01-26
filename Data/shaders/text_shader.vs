#version 330 core
layout (location = 0) in vec3 pos; // <vec2 pos, vec2 tex>
layout (location = 2) in vec2 uv; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(pos.xy, 0.0, 1.0);
    TexCoords   = uv;
}