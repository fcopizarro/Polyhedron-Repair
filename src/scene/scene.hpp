#pragma once

#include "common.hpp"

#include "camera/Camera.hpp"
#include "shader/Shader.hpp"
#include "models/models.hpp"
#include "mesh/octree.hpp"
#include "ui/ui.hpp"
#include "mesh/octree.hpp"


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
    void LoadObject(const std::string &, glm::vec3);


    Camera* camera;
    Model* model;
    Model* model2;
    bool model_in_scene = false;

    
    glm::vec3 lightDir;
    glm::vec3 lightColor;
    glm::vec3 objectColor;

};
