#include "events.hpp"

void Event::event()
{
    // nada
}

/**
 * @brief Deshace el mapeo de coordenadas de ventana a coordenadas del mundo.
 * 
 * Esta función toma una coordenada en la ventana y la convierte a coordenadas del mundo usando las matrices de
 * modelo, vista y proyección proporcionadas.
 * 
 * @param win Coordenadas en la ventana (en píxeles).
 * @param modelView Matriz de modelo-vista.
 * @param projection Matriz de proyección.
 * @param viewport Vista (viewport) de la ventana.
 * @return Coordenadas en el mundo.
 */
glm::vec3 unproject(const glm::vec3& win, const glm::mat4& modelView, const glm::mat4& projection, const glm::ivec4& viewport) {
    glm::mat4 inverse = glm::inverse(projection * modelView);
    glm::vec4 tmp(win.x / float(viewport[2]) * 2.0f - 1.0f, win.y / float(viewport[3]) * 2.0f - 1.0f, 2.0f * win.z - 1.0f, 1.0f);
    glm::vec4 obj = inverse * tmp;
    obj /= obj.w;
    return glm::vec3(obj);
}


/**
 * @brief Obtiene un rayo desde el mouse en coordenadas del mundo.
 * 
 * Calcula el rayo que pasa desde la posición del mouse en la pantalla y lo transforma en coordenadas del mundo,
 * usando las matrices de proyección y vista proporcionadas.
 * 
 * @param mouseX Coordenada X del mouse en la pantalla.
 * @param mouseY Coordenada Y del mouse en la pantalla.
 * @param screenWidth Ancho de la pantalla.
 * @param screenHeight Alto de la pantalla.
 * @param projection Matriz de proyección.
 * @param view Matriz de vista.
 * @return Dirección del rayo en coordenadas del mundo.
 */
glm::vec3 getRayFromMouse(int mouseX, int mouseY, int screenWidth, int screenHeight, const glm::mat4& projection, const glm::mat4& view) {
    // Normalized Device Coordinates
    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    // Eye Coordinates
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // World Coordinates
    glm::vec4 rayWorld = glm::inverse(view) * rayEye;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
    return rayDirection;
}



/**
 * @brief Selecciona el vértice más cercano al rayo generado desde el mouse.
 * 
 * Calcula cuál de los vértices de la malla está más cerca del rayo proyectado desde la posición del mouse en la
 * pantalla, usando un umbral de distancia.
 * 
 * @param mouseX Coordenada X del mouse en la pantalla.
 * @param mouseY Coordenada Y del mouse en la pantalla.
 * @param screenWidth Ancho de la pantalla.
 * @param screenHeight Alto de la pantalla.
 * @param projection Matriz de proyección.
 * @param view Matriz de vista.
 * @param vertices Vector de vértices a examinar.
 * @return Índice del vértice más cercano, o -1 si no se encuentra ninguno dentro del umbral.
 */
int pickVertex(int mouseX, int mouseY, int screenWidth, int screenHeight, const glm::mat4& projection, const glm::mat4& view, std::vector <Vertex> vertices) {
    glm::vec3 rayOrigin = glm::inverse(view)[3]; // La posición de la cámara
    glm::vec3 rayDirection = getRayFromMouse(mouseX, mouseY, screenWidth, screenHeight, projection, view);

    float closestDistance = std::numeric_limits<float>::max();
    glm::vec3 closestVertex;
    int closestIndex;

    for (size_t i = 0; i < vertices.size(); i += 1) {
        glm::vec3 vertex(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
        glm::vec3 rayToPoint = vertex - rayOrigin;
        float projectionLength = glm::dot(rayToPoint, rayDirection);
        glm::vec3 projectedPoint = rayOrigin + projectionLength * rayDirection;
        float distance = glm::length(projectedPoint - vertex);

        if (distance < closestDistance && distance < 0.1f) {  // umbral de distancia
            closestDistance = distance;
            closestVertex = vertex;
            closestIndex = i;
        }
    }

    if (closestDistance < std::numeric_limits<float>::max()) {
        return closestIndex;
    } else {
        return -1;
    }
}


/**
 * @brief Maneja los eventos de entrada.
 * 
 * Procesa los eventos de entrada de SDL, incluyendo el cierre de la aplicación y las interacciones del mouse.
 * También gestiona los eventos de la interfaz de usuario (UI) usando ImGui.
 * 
 * @param quit Referencia a una variable booleana que indica si se debe salir de la aplicación.
 * @param scene Referencia al objeto `Scene` que contiene los datos de la escena actual.
 * @param ui Referencia al objeto `UI` que gestiona la interfaz de usuario.
 */
void Event::handle(bool &quit, Scene& scene, UI& ui )
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }

        scene.camera->Inputs(e);

        ImGui_ImplSDL2_ProcessEvent(&e);



        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                if(const auto& io = ImGui::GetIO(); !io.WantCaptureMouse) {
                
                    if (scene.mesh_in_scene)
                    {
                        ui.closestIndex = pickVertex(e.button.x, e.button.y, ui.width, ui.height, scene.camera->GetProjection(), scene.camera->GetView(), scene.mesh->get_vertices());
                        ui.vertice = scene.mesh->get_vertices();
                    }
                }
            }
        }
        
    }
}
