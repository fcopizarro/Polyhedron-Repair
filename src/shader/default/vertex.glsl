#version 330 core

layout(location = 0) in vec3 aPos;    // Posición del vértice
layout(location = 1) in vec3 aNormal; // Normal del vértice
layout(location = 2) in vec3 aColor; // Normal del vértice


out vec3 Normal;      // Normal que pasaremos al fragment shader
out vec3 FragPos;     // Posición del fragmento en el espacio mundial
out vec3 Color;

uniform mat4 model;       // Matriz de modelo
uniform mat4 view;        // Matriz de vista
uniform mat4 projection;  // Matriz de proyección

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Transformar normal al espacio mundial
    Color = aColor;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
