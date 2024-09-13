#pragma once

#include "common.hpp"
#include "camera/Camera.hpp"
#include "shader/Shader.hpp"
#include "mesh/polyhedral.hpp"
#include "rw/Reader.hpp"

/**
 * @brief Representa una malla 3D.
 *
 * La clase `Mesh` encapsula los datos y métodos necesarios para manejar y renderizar una malla 3D,
 * incluyendo sus vértices, caras y caja de contorno. También proporciona funcionalidad para dibujar la
 * malla y sus normales, actualizar las posiciones de los vértices y gestionar los datos poliedrales asociados.
 */
class Mesh
{
public:

	Mesh(const std::string& file_);


	void Draw(Shader& shader, Camera& camera, bool, int);

	void Draw_normals();

	std::vector <Vertex> get_vertices();
	std::vector <Tri> get_tris();

	std::string filename;
	BoundingBox boundary;

	void setVertexPosition(int index, float x, float y, float z);

	Polyhedral_Mesh polyMesh;
	
	// Mapa para asociar cada vértice con una lista de pares (poliedro, índice del vértice en el poliedro)
        std::unordered_map<glm::vec3*, std::vector<std::pair<Polyhedron*, size_t>>> vertex_to_polyhedron_map;


	void updateNormals();
	void UpdateMeshLines();
	void UpdateModelGraph();

	std::vector <Tri> tris;
	std::vector <Vertex> vertices;

	std::vector<glm::vec3> lineVertices;
	
	glm::vec3 GetCenter();


private:
	void calculateFaceNormals(std::vector<Vertex>& vertices, const std::vector<Tri>& triangles);
	void renderNormals(const std::vector<Vertex>& vertices);
	
	void silhouette();
	void BindShader();
	void DrawMeshLines();

        GLuint ID_VAO;
	GLuint ID_VBO;
        GLuint ID_EBO;

	void BindMeshLines();

	GLuint vaoLines;
	GLuint vboLineVertices;

	GLuint vaoMeshLines;
	GLuint vboMeshLines;
	GLuint Normal_VBO;
	glm::vec3 min;
	glm::vec3 max;


	
};
