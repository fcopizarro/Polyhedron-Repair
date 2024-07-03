#include "mesh.hpp"


void Mesh::renderNormals(const std::vector<Vertex>& vertices) {
    
    std::vector<glm::vec3> lineVertices;
    for (const auto& vertex : vertices) {
        lineVertices.push_back(vertex.position);
        lineVertices.push_back(vertex.position + vertex.normal * 0.1f); // Escalar normal para visualización
    }

    glGenVertexArrays(1, &vaoLines);
    glBindVertexArray(vaoLines);

    GLuint vboLineVertices;

    glGenBuffers(1, &vboLineVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboLineVertices);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(glm::vec3), lineVertices.data(), GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(0); // Desenlazar VAO
}



Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <Tri>& tris, glm::vec3 min, glm::vec3 max)
{


    Mesh::vertices = vertices;
	Mesh::tris = tris;
    calculateFaceNormals(vertices, tris);

    glGenVertexArrays(1, &ID_VAO);
    glBindVertexArray(ID_VAO);

    glGenBuffers(1, &ID_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    
    glGenBuffers(1, &ID_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(Tri), tris.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    renderNormals(vertices);



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


void Mesh::Draw
(
	Shader& shader, 
	Camera& camera,
    bool wireframe,
    bool editmode
)
{
    if (wireframe)
        glPolygonMode(GL_FRONT, GL_LINE);

    glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 0);
    shader.Activate();

	glBindVertexArray(ID_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
    glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (editmode)
    {
    glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 1);
    glBindVertexArray(ID_VAO);
    glPointSize(10.0f); // Configurar tamaño del punto
    glDrawArrays(GL_POINTS, 0, vertices.size());

    glBindVertexArray(0);
    }

}

void Mesh::setVertexPosition(int index, float x, float y, float z)
{
    vertices[index].position = glm::vec3(x, y, z);

    calculateFaceNormals(vertices, tris);

    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


}


void Mesh::Draw_normals()
{
    glBindVertexArray(vaoLines);
    glDrawArrays(GL_LINES, 0, vertices.size() * 2);
    
    glBindVertexArray(0);
}