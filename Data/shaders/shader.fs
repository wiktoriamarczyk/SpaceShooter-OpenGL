#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse0;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect function expects the first vector to point from the light source towards the fragment's position
    vec3 reflectDir = reflect(-lightDir, norm);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // for fragment's normal vector and lightDir, the dot product returns the cosine of the angle between the two vectors
    // if the angle is greater than 90 degrees, the dot product will be negative, so we use max to clamp it to 0
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    float shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * texture(texture_diffuse0, TexCoord).rgb;
    FragColor = vec4(result, 1.0);

}