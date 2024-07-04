#version 330 core

layout(location = 0) in vec3 aPos;    // Posición del vértice
layout(location = 1) in vec3 aNormal; // Normal del vértice
layout(location = 2) in vec3 aColor;
layout(location = 3) in float aType;


flat out vec3 Normal;      // Normal que pasaremos al fragment shader
out vec3 FragPos;     // Posición del fragmento en el espacio mundial
flat out vec3 Color;
flat out vec3 ReflectColor;

flat out float vType;

uniform mat4 model;       // Matriz de modelo
uniform mat4 view;        // Matriz de vista
uniform mat4 projection;  // Matriz de proyección

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Transformar normal al espacio mundial
    Color = aColor;
    vType = aType;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
