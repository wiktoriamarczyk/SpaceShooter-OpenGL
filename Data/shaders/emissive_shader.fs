#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture_diffuse0;
uniform vec3 color;

void main()
{
    vec4 result = texture(texture_diffuse0, TexCoord);
	result.xyz *= color;
	FragColor = result;
}