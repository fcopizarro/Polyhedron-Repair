#pragma once    

#include "common.hpp"

#include "camera/Camera.hpp"
#include "shader/shader.hpp"

class Mesh
{
public:
	std::vector <Tri> tris;
	std::vector <Vertex> vertices;
	//BoundingBox boundigBoxMesh;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <Tri>& tris, glm::vec3 min, glm::vec3 max);
    //Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh
	void Draw
	(
		Shader& shader, 
		Camera& camera,
		bool wireframe,
		bool editmode
	);

	void Draw_normals();

    /*
    void Draw
	(
		Shader& shader, 
		Camera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
    */

   	void setVertexPosition(int index, float x, float y, float z);
private:
    GLuint ID_VAO;
	GLuint vaoLines;
    GLuint ID_VBO;
    GLuint ID_EBO;
	GLuint Normal_VBO;

	void calculateFaceNormals(std::vector<Vertex>& vertices, const std::vector<Tri>& triangles);
	void renderNormals(const std::vector<Vertex>& vertices);
};