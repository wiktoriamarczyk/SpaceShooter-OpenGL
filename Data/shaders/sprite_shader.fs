#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 color;
uniform sampler2D texture_diffuse0;

void main()
{
    vec4 result = texture(texture_diffuse0, TexCoord);
	result *= color;
	FragColor = result;
}