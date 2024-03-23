#include "Camera.hpp"


Camera::Camera()
{
    this->cameraAngleX = 0.0f;
    this->cameraAngleY = 0.0f;
    this->sensitivity = 0.1f;
}
Camera::Camera(float cameraAngleX, float cameraAngleY, float sensitivity)
{
    this->cameraAngleX = cameraAngleX;
    this->cameraAngleY = cameraAngleY;
    this->sensitivity = sensitivity;
}


// Función para rotar la cámara con el mouse
void Camera::EventListener(SDL_Event& event) {
    // Manejar eventos del mouse
    if (event.type == SDL_MOUSEMOTION) {
        // Obtener los desplazamientos del mouse
        int deltaX = event.motion.xrel;
        int deltaY = event.motion.yrel;

        // Aplicar sensibilidad para controlar la velocidad de rotación
        deltaX *= this->sensitivity;
        deltaY *= this->sensitivity;

        // Actualizar los ángulos de la cámara basados en los desplazamientos del mouse
        this->cameraAngleY += deltaX;
        this->cameraAngleX += deltaY;

        // Limitar el ángulo vertical de la cámara entre -90 y 90 grados para evitar que la cámara rote demasiado hacia arriba o hacia abajo
        if (this->cameraAngleX > 90.0f) {
            this->cameraAngleX = 90.0f;
        } else if (this->cameraAngleX < -90.0f) {
            this->cameraAngleX = -90.0f;
        }
    }
}

void Camera::UpdateCamera()
{
    glRotatef(this->cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(this->cameraAngleY, 0.0f, 1.0f, 0.0f);
}