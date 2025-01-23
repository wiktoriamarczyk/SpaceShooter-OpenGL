#version 330 core

#define MAX_POINT_LIGHTS 128

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight
{
    vec3 position;

    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight
{
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
};

uniform vec3 ambient;
uniform sampler2D texture_diffuse0;
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform int pointLightsCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = ambient;

    // calculate directional light
    result += CalcDirLight(dirLight, norm, viewDir);

    // do the same for all point lights
    for (int i = 0; i < min(pointLightsCount, MAX_POINT_LIGHTS-1); i++)
    {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    result = min(result, vec3(1.0, 1.0, 1.0));
    result *= texture(texture_diffuse0, TexCoord).rgb;
	
	result *= Color;

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 diffuse  = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;

    diffuse  *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}