#version 330 core

in vec3 Normal;  // Normal del fragmento
in vec3 FragPos; // Posición del fragmento
in vec3 Color;
flat in float vType;

out vec4 FragColor;


uniform vec3 lightPos;   // Posición de la luz
uniform vec3 lightDir;
uniform vec3 viewPos;    // Posición de la cámara
uniform vec3 lightColor; // Color de la luz
uniform vec3 objectColor;// Color del objeto
uniform int isVertex;


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
    //vec3 result = (ambient + diffuse + specular)*Color;
    vec3 result = Color;

    vec2 coord = gl_PointCoord;

    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));

    float intensity = max(dot(normalize(Normal), normalize(viewPos - FragPos)), 0.0);

    vec3 reflectionColor = vec3(1.0, 1.0, 1.0);
    vec3 color2 = reflectionColor * intensity;

    if (isVertex  == 3.0) // Vertice seleccionado
    {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0); // Color verde para puntos
    } else if (isVertex  == 1.0) // Vertices no seleccionados
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Color verde para puntos
    } else if (isVertex == 2.0)
    {
        FragColor = vec4(0.0, 0.0, 0.0 , 1.0); 
    } else
    {
        //FragColor = vec4(result, 1.0);
        FragColor = vec4(color2, 1.0);
    }
}
