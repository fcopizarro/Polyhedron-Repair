#include "events.hpp"

void Event::event()
{
    // nada
}


glm::vec3 unproject(const glm::vec3& win, const glm::mat4& modelView, const glm::mat4& projection, const glm::ivec4& viewport) {
    glm::mat4 inverse = glm::inverse(projection * modelView);
    glm::vec4 tmp(win.x / float(viewport[2]) * 2.0f - 1.0f, win.y / float(viewport[3]) * 2.0f - 1.0f, 2.0f * win.z - 1.0f, 1.0f);
    glm::vec4 obj = inverse * tmp;
    obj /= obj.w;
    return glm::vec3(obj);
}

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
                
                    if (scene.model_in_scene)
                    {
                        ui.closestIndex = pickVertex(e.button.x, e.button.y, ui.width, ui.height, scene.camera->GetProjection(), scene.camera->GetView(), scene.model->get_vertices());
                        ui.vertice = scene.model->get_vertices();
                    }
                }
            }
        }
        
    }
}