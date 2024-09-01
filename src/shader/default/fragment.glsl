#version 330 core

flat in vec3 Normal;  // Normal del fragmento
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

    vec3 norm = Normal;

    vec3 result = Color;

    vec2 coord = gl_PointCoord;

    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, norm);

    float intensity = max(dot(norm, -I), 0.0);

    vec3 reflectionColor = vec3(1.0, 1.0, 1.0);
    reflectionColor *= intensity;

    if (isVertex  == 3.0) // Vertice seleccionado
    {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0); // Color verde para puntos
    } else if (isVertex  == 1.0) // Vertices no seleccionados
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Color verde para puntos
    } else if (isVertex == 2.0) // Para dibujar Lineas
    {
        FragColor = vec4(0.0, 0.0, 0.0 , 1.0); 
    } else
    {
        //FragColor = vec4(result, 1.0);
        FragColor = vec4(reflectionColor + vec3(0.7,0.7,0.7), 1.0);
    }
}