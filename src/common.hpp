#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>
#include <imfilebrowser.h>

#include <vector>
#include <cmath>
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <limits>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <tuple>



/**
 * @struct Vertex
 * @brief Representa un vértice en el espacio 3D.
 * 
 * Esta estructura almacena la posición y la normal del vértice, así como un contador opcional.
 */
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    int count = 0;

     /**
     * @brief Constructor de Vertex con posición y normal.
     * 
     * @param position La posición del vértice en el espacio 3D.
     * @param normal La normal del vértice.
     */
    Vertex(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) {}

     /**
     * @brief Constructor de Vertex con solo posición.
     * 
     * @param position La posición del vértice en el espacio 3D.
     */
    Vertex(glm::vec3 position) : position(position) {}
};


/**
 * @struct Tri
 * @brief Representa un triángulo definido por tres vértices.
 * 
 * Esta estructura almacena los índices de los tres vértices que forman el triángulo.
 */
struct Tri {
    int v0, v1, v2;
};



/**
 * @struct BoundingBox
 * @brief Representa una caja delimitadora en el espacio 3D.
 * 
 * Esta estructura define una caja delimitadora utilizando los vectores mínimos y máximos en las tres dimensiones.
 */
struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;

     /**
     * @brief Constructor por defecto de BoundingBox.
     * 
     * Inicializa la caja delimitadora con mínimos y máximos en el origen (0.0f, 0.0f, 0.0f).
     */
    BoundingBox() : min(glm::vec3(0.0f, 0.0f, 0.0f)), max(glm::vec3(0.0f, 0.0f, 0.0f)) {}

    /**
     * @brief Constructor de BoundingBox con valores específicos.
     * 
     * @param min El punto más bajo (mínimo) de la caja delimitadora.
     * @param max El punto más alto (máximo) de la caja delimitadora.
     */
    BoundingBox(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
};
