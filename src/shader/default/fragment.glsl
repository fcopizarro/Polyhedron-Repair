#version 330 core

in vec3 Normal;  // Normal del fragmento
in vec3 FragPos; // Posición del fragmento

out vec4 FragColor;

uniform vec3 lightPos;   // Posición de la luz
uniform vec3 lightDir;
uniform vec3 viewPos;    // Posición de la cámara
uniform vec3 lightColor; // Color de la luz
uniform vec3 objectColor;// Color del objeto

void main() {
    // Propiedades ambientales
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Propiedades difusas
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Suma de los componentes
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
