#pragma once

#include "common.hpp"
#include "camera/Camera.hpp"
#include "shader/Shader.hpp"


#include "mesh/mesh.hpp"
#include "mesh/octree.hpp"
#include "models/polyhedral.hpp"



class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const std::string& file_, glm::vec3);
	Model(const OctreeNode* octree, glm::vec3);

	void Draw(Shader& shader, Camera& camera, bool wireframe, bool);

	void Draw_normals();

	std::vector <Vertex> get_vertices();
	std::vector <Tri> get_tris();

	std::string filename;
	BoundingBox boundary;

	void setVertexPosition(int index, float x, float y, float z);

	Polyhedral_Mesh polyMesh;

	void updateNormals();
	void UpdateMeshLines();
	void UpdateModelGraph();

	std::vector <Tri> tris;
	std::vector <Vertex> vertices;

	std::vector<glm::vec3> lineVertices;


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
	// Variables for easy access

    std::string obtenerExtension(const std::string&);
    void read_obj(const std::string&, glm::vec3);
    void read_vtk(const std::string&, glm::vec3);
	std::vector<unsigned char> data;

	
	
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Prevents textures from being loaded twice
	std::vector<std::string> loadedTexName;
	//std::vector<Texture> loadedTex;

	// Loads a single mesh by its index
	void loadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> getData();
	
};