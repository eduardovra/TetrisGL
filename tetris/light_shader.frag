#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 viewPos;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 position, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);

    result += CalcDirLight(norm, viewDir);

    vec3 pointLightPosition1 = vec3(-4.0, -4.0, -2.0);
    result += CalcPointLight(pointLightPosition1, norm, FragPos, viewDir);

    vec3 pointLightPosition2 = vec3(4.0, 4.0, -2.0);
    result += CalcPointLight(pointLightPosition2, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDirection = vec3(-0.2, -0.5, -0.3);
    vec3 lightDir = normalize(-lightDirection);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // combine results
    vec3 ambient  = vec3(0.2, 0.2, 0.2)  * color;
    vec3 diffuse  = vec3(0.2, 0.2, 0.2)  * diff * color;
    vec3 specular = vec3(0.8, 0.8, 0.8) * spec * color;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(vec3 position, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // attenuation
    float distance    = length(position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    // combine results
    vec3 ambient  = vec3(0.2, 0.2, 0.2)  * color;
    vec3 diffuse  = vec3(0.5, 0.5, 0.5)  * diff * color;
    vec3 specular = vec3(1.0, 1.0, 1.0) * spec * color;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
