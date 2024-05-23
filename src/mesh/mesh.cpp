#include "mesh.hpp"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <Tri>& tris)
{


    Mesh::vertices = vertices;
	Mesh::tris = tris;
    calculateFaceNormals(vertices, tris);

    glGenVertexArrays(1, &ID_VAO);
    glBindVertexArray(ID_VAO);

    glGenBuffers(1, &ID_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    //glGenBuffers(1, &Normal_VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, Normal_VBO);
    //glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    
    glGenBuffers(1, &ID_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(Tri), tris.data(), GL_STATIC_DRAW);


    //glBindBuffer(GL_ARRAY_BUFFER, Normal_VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void Mesh::calculateFaceNormals(std::vector<Vertex>& vertices, const std::vector<Tri>& triangles)
{

    for (const auto& triangle : triangles) {
        glm::vec3& v0 = vertices[triangle.v0].position;
        glm::vec3& v1 = vertices[triangle.v1].position;
        glm::vec3& v2 = vertices[triangle.v2].position;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        vertices[triangle.v0].normal += normal;
        vertices[triangle.v1].normal += normal;
        vertices[triangle.v2].normal += normal;
    }

    for (auto& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}

void renderNormals(const std::vector<Vertex>& vertices) {
    glBegin(GL_LINES);
    for (const auto& vertex : vertices) {
        glVertex3fv(&vertex.position.x);
        glm::vec3 normalEnd = vertex.position + vertex.normal * 10.0f;  // Escala para visualización
        glVertex3fv(&normalEnd.x);
    }
    glEnd();
}



/*
void Mesh::Draw
(
	Shader& shader, 
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation, 
	glm::quat rotation, 
	glm::vec3 scale
)
*/
void Mesh::Draw
(
	Shader& shader, 
	Camera& camera
)
{
    shader.Activate();
	glBindVertexArray(ID_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
    glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    renderNormals(vertices);


}
