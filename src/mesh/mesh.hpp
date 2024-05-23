#pragma once    

#include "common.hpp"

#include "Camera.hpp"
#include "shader/shader.hpp"

class Mesh
{
public:
	//std::vector <glm::vec3> vertices;
	//std::vector <GLuint> indices;
	//std::vector <glm::vec3> normals;

	std::vector <Tri> tris;
	std::vector <Vertex> vertices;

	//std::vector <Tri> tris;
	//std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	//VAO VAO;

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <Tri>& tris);
    //Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Draws the mesh
	void Draw
	(
		Shader& shader, 
		Camera& camera
	);

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
private:
    GLuint ID_VAO;
    GLuint ID_VBO;
    GLuint ID_EBO;
	GLuint Normal_VBO;

	void calculateFaceNormals(std::vector<Vertex>& vertices, const std::vector<Tri>& triangles);
};