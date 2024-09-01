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
    

    glUniform3f(glGetUniformLocation(shader.ID, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), objectColor.x, objectColor.y, objectColor.z);

    
        


    camera->Matrix(45.0f, 0.001f, 100.0f, shader, "camMatrix");

    glUniform3f(glGetUniformLocation(shader.ID, "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);

    //GraphAxis( shader);

    if (ui.createOctree)
    {
        OctreeNode* octree = new OctreeNode(BoundingBox (model->boundary.min, model->boundary.max), 0, 5);
        octree->subdivide(model->get_vertices(), model->get_tris());
        //Model octree_model(octree);
        model2 = new Model(octree, glm::vec3(1.0f));
        std::cout << model->get_vertices().size() <<  std::endl;
        //models_loaded.push_back(octree_model);
        ui.createOctree = false;
        octreeCreated = true;
    }

    if (model_in_scene)
    {
        model->Draw(shader, *camera, false, ui.editMode);
        if (octreeCreated)
        {
            model2->Draw(shader, *camera, false, ui.editMode);
        }

        ui.qtyHexa = model->polyMesh.qtyHexa;
        ui.qtyPrism = model->polyMesh.qtyPrism;
        ui.qtyPyra = model->polyMesh.qtyPyra;
        ui.qtyTetra = model->polyMesh.qtyTetra;

        camera->cameraTarget = glm::vec3( (model->boundary.min.x + model->boundary.max.x) / 2.0f, (model->boundary.min.y + model->boundary.max.y) / 2.0f, (model->boundary.min.z + model->boundary.max.z) / 2.0f  );
   
    
        
        if(ui.includeHexa != model->polyMesh.includeHexa 
        || ui.includeTetra != model->polyMesh.includeTetra
        || ui.includePrism != model->polyMesh.includePrism
        || ui.includePyra != model->polyMesh.includePyra
        || ui.includeHexa2 != model->polyMesh.includeHexa2
        || ui.includeTetra2 != model->polyMesh.includeTetra2
        || ui.includePrism2 != model->polyMesh.includePrism2
        || ui.includePyra2 != model->polyMesh.includePyra2
        
            )
        {
            model->polyMesh.includeHexa = ui.includeHexa;
            model->polyMesh.includeTetra = ui.includeTetra;
            model->polyMesh.includePrism = ui.includePrism;
            model->polyMesh.includePyra = ui.includePyra;
            model->polyMesh.includeHexa2 = ui.includeHexa2;
            model->polyMesh.includeTetra2 = ui.includeTetra2;
            model->polyMesh.includePrism2 = ui.includePrism2;
            model->polyMesh.includePyra2 = ui.includePyra2;
            // calculatej cuando hay cambio de vert
            model->polyMesh.GetJ();
            ui.histogramData = model->polyMesh.Jtotal;
            ui.Jdata = model->polyMesh.Jdata;
            ui.JRdata = model->polyMesh.JRdata;
            ui.JENSdata = model->polyMesh.JENSdata;
            ui.EQdata = model->polyMesh.EQdata;

            ui.ARtotal = model->polyMesh.ARtotal;
            ui.ARdata = model->polyMesh.ARdata;
            ui.ARGdata = model->polyMesh.ARGdata;
            ui.ARENdata = model->polyMesh.ARENdata;
        }

    }

    
}

void Scene::LoadObject(const std::string& file_, glm::vec3 color)
{
    model = new Model(file_, color);
    model_in_scene = true;
    

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