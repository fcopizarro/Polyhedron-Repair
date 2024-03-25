#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, GLuint shaderID, const char* uniform)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}


void Camera::Inputs(SDL_Event& event)
{
    
    if (event.type == SDL_KEYDOWN) {

        switch (event.key.keysym.sym)
        {
        case SDLK_a:
            Position += speed * -glm::normalize(glm::cross(Orientation, Up));
            break;
        case SDLK_w:
            Position += speed * Orientation;

        case SDLK_s:
            Position += speed * -Orientation;
            break;
        case SDLK_d:
            Position += speed * glm::normalize(glm::cross(Orientation, Up));
        
        default:
            break;
        }
    }

    /*
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}
    */

    /*

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
    */
}


/*<
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
*/