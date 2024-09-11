#pragma once

#include "common.hpp"

#include "camera/Camera.hpp"
#include "shader/Shader.hpp"
#include "mesh/mesh.hpp"
#include "ui/ui.hpp"



class Scene
{
private:


    unsigned int width;
    unsigned int height;	


    GLuint VAO_axis;
    void BindAxis();
    void GraphAxis(Shader& shader);

    bool octreeCreated = false;


public:
    Scene(const unsigned int width, const unsigned int height);
    void Update(Shader& ,UI&);
    void LoadObject(const std::string &);


    Camera* camera;
    Mesh* mesh;
    bool mesh_in_scene = false;

    
    glm::vec3 lightDir;
    glm::vec3 lightColor;
    glm::vec3 objectColor;
    
    bool isMeshLoaded();
    const char* getMeshName() const;
    size_t getVertexCount();
    size_t getPolyhedronCount();
    
    int getHexaCount();
    int getTetraCount();
    int getPrismCount();
    int getPyraCount();

    void CalculateMetrics();
    std::vector<float>* getJacobianTotalPtr();
    std::vector<float>* getJsPtr();
    std::vector<float>* getJensPtr();
    std::vector<float>* getARTotalPtr();
    std::vector<float>* getARPtr();
    std::vector<float>* getARGPtr();
    std::vector<float>* getARenPtr();



};
