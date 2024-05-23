#pragma once

#include "common.hpp"
#include "Camera.hpp"
#include "shader/Shader.hpp"


#include "mesh/mesh.hpp"


class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const std::string& file);

	void Draw(Shader& shader, Camera& camera);

private:
	// Variables for easy access

    std::string obtenerExtension(const std::string&);
    void read_obj(const std::string&);
    void read_vtk(const std::string&);
	const char* file;
	std::vector<unsigned char> data;

	// All the meshes and transformations
	std::vector<Mesh> meshes;
    //std::vector<glm::vec3> vertices;
	//std::vector<Vertex> normales;
    std::vector<GLuint> tris2;

	std::vector <Tri> tris;
	std::vector <Vertex> vertices;

	
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
	// Interprets the binary data into floats, indices, and textures
    /*
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();
    */

	// Assembles all the floats into vertices
    /*
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions, 
		std::vector<glm::vec3> normals, 
		std::vector<glm::vec2> texUVs
	);
    */

    /*

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);*/
};