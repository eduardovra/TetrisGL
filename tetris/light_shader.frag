#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 color;
uniform vec3 viewPos;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(norm, viewDir);

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
    vec3 ambient  = vec3(0.05f, 0.05f, 0.05f)  * color;
    vec3 diffuse  = vec3(0.8f, 0.8f, 0.8f)  * diff * color;
    vec3 specular = vec3(0.5f, 0.5f, 0.5f) * spec * color;

    return (ambient + diffuse + specular);
}
