#include "scene.hpp"


Scene::Scene(const unsigned int width, const unsigned int height)
{
    camera = new Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    lightDir = glm::vec3(2.0f, 2.0f, 2.0f);
    lightColor = glm::vec3(2.0f, 2.0f, 2.0f);
    objectColor = glm::vec3(0.0f, 0.0f, 2.0f);


    Scene::BindAxis();

}




void Scene::Update(Shader& shader, UI& ui)
{
    shader.Activate();


    // TODO: Cambiar metodos a la camara
    // camera -> Update();
    camera->Matrix(45.0f, 0.001f, 100.0f, shader, "camMatrix");
    glUniform3f(glGetUniformLocation(shader.ID, "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
    
    if (mesh_in_scene)
    {
        mesh->Draw(shader, *camera, ui.editMode, ui.closestIndex);

        camera->cameraTarget = mesh->GetCenter();
   
    
        
        if(ui.includeHexa != mesh->polyMesh.includeHexa 
        || ui.includeTetra != mesh->polyMesh.includeTetra
        || ui.includePrism != mesh->polyMesh.includePrism
        || ui.includePyra != mesh->polyMesh.includePyra
        || ui.includeHexa2 != mesh->polyMesh.includeHexa2
        || ui.includeTetra2 != mesh->polyMesh.includeTetra2
        || ui.includePrism2 != mesh->polyMesh.includePrism2
        || ui.includePyra2 != mesh->polyMesh.includePyra2
        
            )
        {
            mesh->polyMesh.includeHexa = ui.includeHexa;
            mesh->polyMesh.includeTetra = ui.includeTetra;
            mesh->polyMesh.includePrism = ui.includePrism;
            mesh->polyMesh.includePyra = ui.includePyra;
            mesh->polyMesh.includeHexa2 = ui.includeHexa2;
            mesh->polyMesh.includeTetra2 = ui.includeTetra2;
            mesh->polyMesh.includePrism2 = ui.includePrism2;
            mesh->polyMesh.includePyra2 = ui.includePyra2;
            // calculatej cuando hay cambio de vert
            //mesh->polyMesh.CalculateJ();
        }

    }

    
}

void Scene::LoadObject(const std::string& file_)
{
    mesh = new Mesh(file_);
    mesh_in_scene = true;
    

}

void Scene::BindAxis()
{
    GLfloat vert[] = {
        // Positions        // Colors
         0.0f, 0.0f, 0.0f,  2.0f, 0.0f, 0.0f, // X axis: Red
         2.0f, 0.0f, 0.0f,  2.0f, 0.0f, 0.0f,
         
         0.0f, 0.0f, 0.0f,  0.0f, 2.0f, 0.0f, // Y axis: Green
         0.0f, 2.0f, 0.0f,  0.0f, 2.0f, 0.0f,
         
         0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 2.0f, // Z axis: Blue
         0.0f, 0.0f, 2.0f,  0.0f, 0.0f, 2.0f,
    };


    GLuint VBO;
    glGenVertexArrays(1, &VAO_axis);
    glGenBuffers(1, &VBO);

    // Bind and set up VAO and VBO
    glBindVertexArray(VAO_axis);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Vertex colors
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Scene::GraphAxis(Shader& shader)
{
    
    glBindVertexArray(VAO_axis);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);


}

bool Scene::isMeshLoaded() {return mesh_in_scene;} 
const char* Scene::getMeshName() const {return mesh->filename.c_str();}
size_t Scene::getVertexCount() {return mesh->vertices.size();} 
size_t Scene::getPolyhedronCount() {return mesh->polyMesh.polyhedrons.size();} 

int Scene::getHexaCount() {return mesh->polyMesh.qtyHexa;} 
int Scene::getTetraCount() {return mesh->polyMesh.qtyTetra;} 
int Scene::getPrismCount() {return mesh->polyMesh.qtyPrism;} 
int Scene::getPyraCount() {return mesh->polyMesh.qtyPyra;} 

void Scene::CalculateMetrics() {mesh->polyMesh.CalculateJ();} 

std::vector<float>* Scene::getJacobianTotalPtr() { return &mesh->polyMesh.Jtotal;}
std::vector<float>* Scene::getJsPtr() {return &(mesh->polyMesh.Jdata);}
std::vector<float>* Scene::getJensPtr() {return &(mesh->polyMesh.JENSdata);}
std::vector<float>* Scene::getARTotalPtr() {return &(mesh->polyMesh.ARtotal);}
std::vector<float>* Scene::getARPtr() {return &(mesh->polyMesh.ARdata);}
std::vector<float>* Scene::getARGPtr() {return &(mesh->polyMesh.ARGdata);}
std::vector<float>* Scene::getARenPtr() {return &(mesh->polyMesh.ARENdata);}
