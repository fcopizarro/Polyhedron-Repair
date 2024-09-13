#include "scene.hpp"

/**
 * @brief Constructor de la clase `Scene`.
 * 
 * Este constructor inicializa una nueva instancia de la clase `Scene`, configurando una cámara con las 
 * dimensiones especificadas por `width` y `height`. La cámara se coloca en la posición `(0.0f, 0.0f, 2.0f)` 
 * en el espacio 3D.
 * 
 * @param width Ancho de la ventana de la escena en píxeles.
 * @param height Altura de la ventana de la escena en píxeles.
 */
Scene::Scene(const unsigned int width, const unsigned int height)
{
    camera = new Camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    //Scene::BindAxis();
}



/**
 * @brief Actualiza la escena y renderiza los objetos visibles.
 * 
 * Este método activa el shader proporcionado y actualiza la matriz de la cámara para renderizar la escena. 
 * Luego, si hay un objeto de malla (`mesh_in_scene`) en la escena, realiza las siguientes acciones:
 * 
 * - Configura la matriz de vista de la cámara en el shader.
 * - Actualiza la posición de la cámara en el shader.
 * - Establece el objetivo de la cámara en el centro de la malla.
 * - Dibuja la malla utilizando el shader, la cámara, y la interfaz de usuario (`UI`).
 * - Verifica si los parámetros de inclusión de diferentes tipos de polígonos han cambiado en la interfaz de usuario. 
 *   Si han cambiado, actualiza los parámetros en la malla y recalcula las métricas de la malla.
 * 
 * @param shader El objeto `Shader` que se utiliza para activar el shader y configurar las matrices.
 * @param ui La interfaz de usuario (`UI`) que proporciona información sobre el modo de edición y las opciones de visualización.
 */
void Scene::Update(Shader& shader, UI& ui)
{
    shader.Activate();

    if (mesh_in_scene)
    {
        // Configura la matriz de vista de la cámara y la posición de la cámara en el shader
        camera->Matrix(45.0f, 0.001f, 100.0f, shader, "camMatrix");
        glUniform3f(glGetUniformLocation(shader.ID, "viewPos"), camera->cameraPos.x, camera->cameraPos.y, camera->cameraPos.z);
        camera->cameraTarget = mesh->GetCenter();

        // Dibuja la malla con el shader y la cámara, incluyendo opciones de edición y selección en la UI
        mesh->Draw(shader, *camera, ui.editMode, ui.closestIndex);
        
        // Verifica si ha cambiado la inclusión de tipos de polígonos y actualiza si es necesario
        if(ui.includeHexa != mesh->polyMesh.includeHexa  // Cambios en las metricas Jacobianas.
        || ui.includeTetra != mesh->polyMesh.includeTetra
        || ui.includePrism != mesh->polyMesh.includePrism
        || ui.includePyra != mesh->polyMesh.includePyra
        || ui.includeHexa2 != mesh->polyMesh.includeHexa2 // Cambios en las metricas AR
        || ui.includeTetra2 != mesh->polyMesh.includeTetra2
        || ui.includePrism2 != mesh->polyMesh.includePrism2
        || ui.includePyra2 != mesh->polyMesh.includePyra2)
        {
            mesh->polyMesh.includeHexa = ui.includeHexa;
            mesh->polyMesh.includeTetra = ui.includeTetra;
            mesh->polyMesh.includePrism = ui.includePrism;
            mesh->polyMesh.includePyra = ui.includePyra;
            mesh->polyMesh.includeHexa2 = ui.includeHexa2;
            mesh->polyMesh.includeTetra2 = ui.includeTetra2;
            mesh->polyMesh.includePrism2 = ui.includePrism2;
            mesh->polyMesh.includePyra2 = ui.includePyra2;
            mesh->polyMesh.CalculateMetrics();
        }

    }
    
}


/**
 * @brief Carga un objeto de malla desde un archivo y lo añade a la escena.
 * 
 * Este método crea una nueva instancia de `Mesh` utilizando el archivo especificado por `file_`. 
 * Luego, marca que hay una malla en la escena (`mesh_in_scene` se establece en `true`), indicando que 
 * la escena ahora contiene un objeto de malla que puede ser renderizado y manipulado.
 * 
 * @param file_ El nombre del archivo desde el cual se cargará el objeto de malla. El archivo debe estar en un formato 
 *              compatible con el constructor de `Mesh`.
 */
void Scene::LoadObject(const std::string& file_)
{
    mesh = new Mesh(file_);
    mesh_in_scene = true;
}


/* METODOS DESHABILITADOS, USADOS PRINCIPALMENTE PARA DEBUGEAR Y VER LOS EJES DEL ESPACIO 3D.
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
*/



/**
 * @brief Verifica si hay una malla cargada en la escena.
 * 
 * @return `true` si hay una malla cargada (`mesh_in_scene` es `true`), de lo contrario `false`.
 */
bool Scene::isMeshLoaded() {return mesh_in_scene;} 

/**
 * @brief Obtiene el nombre del archivo de la malla cargada.
 * 
 * @return Nombre del archivo de la malla como una cadena de caracteres de tipo `const char*`.
 */
const char* Scene::getMeshName() const {return mesh->filename.c_str();}

/**
 * @brief Obtiene el número de vértices en la malla cargada.
 * 
 * @return Cantidad de vértices en la malla.
 */
size_t Scene::getVertexCount() {return mesh->vertices.size();} 

/**
 * @brief Obtiene el número de poliedros en la malla cargada.
 * 
 * @return Cantidad de poliedros en la malla.
 */
size_t Scene::getPolyhedronCount() {return mesh->polyMesh.polyhedrons.size();} 

/**
 * @brief Obtiene la cantidad de hexaedros en la malla cargada.
 * 
 * @return Cantidad de hexaedros en la malla.
 */
int Scene::getHexaCount() {return mesh->polyMesh.qtyHexa;} 

/**
 * @brief Obtiene la cantidad de tetraedros en la malla cargada.
 * 
 * @return Cantidad de tetraedros en la malla.
 */
int Scene::getTetraCount() {return mesh->polyMesh.qtyTetra;} 

/**
 * @brief Obtiene la cantidad de prismas en la malla cargada.
 * 
 * @return Cantidad de prismas en la malla.
 */
int Scene::getPrismCount() {return mesh->polyMesh.qtyPrism;} 

/**
 * @brief Obtiene la cantidad de pirámides en la malla cargada.
 * 
 * @return Cantidad de pirámides en la malla.
 */
int Scene::getPyraCount() {return mesh->polyMesh.qtyPyra;} 

/**
 * @brief Calcula las métricas de la malla cargada.
 * 
 * Llama al método `CalculateMetrics` de `polyMesh` para actualizar las métricas de la malla.
 */
void Scene::CalculateMetrics() {mesh->polyMesh.CalculateMetrics();} 

/**
 * @brief Obtiene un puntero al vector de datos del Jacobiano total.
 * 
 * @return Puntero al vector `Jtotal` de `polyMesh`.
 */
std::vector<float>* Scene::getJacobianTotalPtr() { return &mesh->polyMesh.Jtotal;}

/**
 * @brief Obtiene un puntero al vector de datos del Jacobiano.
 * 
 * @return Puntero al vector `Jdata` de `polyMesh`.
 */
std::vector<float>* Scene::getJsPtr() {return &(mesh->polyMesh.Jdata);}

/**
 * @brief Obtiene un puntero al vector de datos del Jacobiano de la energía de superficie.
 * 
 * @return Puntero al vector `JENSdata` de `polyMesh`.
 */
std::vector<float>* Scene::getJensPtr() {return &(mesh->polyMesh.JENSdata);}

/**
 * @brief Obtiene un puntero al vector de datos de la relación de aspecto total.
 * 
 * @return Puntero al vector `ARtotal` de `polyMesh`.
 */
std::vector<float>* Scene::getARTotalPtr() {return &(mesh->polyMesh.ARtotal);}


/**
 * @brief Obtiene un puntero al vector de datos de la relación de aspecto.
 * 
 * @return Puntero al vector `ARdata` de `polyMesh`.
 */
std::vector<float>* Scene::getARPtr() {return &(mesh->polyMesh.ARdata);}

/**
 * @brief Obtiene un puntero al vector de datos de la relación de aspecto general.
 * 
 * @return Puntero al vector `ARGdata` de `polyMesh`.
 */
std::vector<float>* Scene::getARGPtr() {return &(mesh->polyMesh.ARGdata);}

/**
 * @brief Obtiene un puntero al vector de datos de la relación de aspecto extendida.
 * 
 * @return Puntero al vector `ARENdata` de `polyMesh`.
 */
std::vector<float>* Scene::getARenPtr() {return &(mesh->polyMesh.ARENdata);}
