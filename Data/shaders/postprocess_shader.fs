#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform float saturation;
uniform sampler2D texture_diffuse0;

void main()
{
    vec4 result = texture(texture_diffuse0, TexCoord);
	
	float grey = (result.x+result.y+result.z)/3;
	
	vec3 color = mix(vec3(grey,grey,grey),result.xyz,saturation);
	FragColor = vec4(color,result.w);
}