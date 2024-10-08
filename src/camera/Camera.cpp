#include "Camera.hpp"

/**
 * @brief Constructor de la clase Camera.
 * 
 * Inicializa los parámetros de la cámara y establece su posición inicial, así como el ángulo de inclinación y orientación predeterminados.
 * 
 * @param width Ancho de la ventana de visualización.
 * @param height Alto de la ventana de visualización.
 * @param position Posición inicial de la cámara.
 */
Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
	mYaw = DefaultYaw;
	mPitch = DefaultPitch;

	cameraTarget = glm::vec3(0.0f);
}


/**
 * @brief Configura las matrices de vista y proyección de la cámara y las envía al shader.
 * 
 * Ajusta las matrices de vista y proyección de la cámara según los parámetros proporcionados y el estado de `centerCameraActivated`.
 * 
 * @param FOVdeg Campo de visión de la cámara en grados.
 * @param nearPlane Distancia del plano cercano de recorte.
 * @param farPlane Distancia del plano lejano de recorte.
 * @param shader Objeto `Shader` al que se le asignará la matriz de la cámara.
 * @param uniform Nombre del uniforme en el shader para la matriz.
 */
void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	if (centerCameraActivated)
	{
		view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    	projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		    // Convertir ángulos a radianes
		float azimuthRad = glm::radians(azimuth);
		float elevationRad = glm::radians(elevation);

		// Calcular la posición de la cámara en coordenadas esféricas
		cameraPos.x = radius * cos(elevationRad) * cos(azimuthRad);
		cameraPos.y = radius * sin(elevationRad);
		cameraPos.z = radius * cos(elevationRad) * sin(azimuthRad);

	} else {
	// Initializes matrices since otherwise they will be the null matrix

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	//  glm::lookAt(mPosition, mPosition + mFrontDir, mUpDir);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	}
	// glm::perspective(glm::radians(mZoom), screenSize.x / screenSize.y, .001f, 1000.0f);
	glm::mat4 model = glm::mat4(1.0f); 

	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	

	
}

/**
 * @brief Obtiene la matriz de vista de la cámara.
 * 
 * @return La matriz de vista de la cámara como un `glm::mat4`.
 */
glm::mat4 Camera::GetView()
{
	return view;
}


/**
 * @brief Obtiene la matriz de proyección de la cámara.
 * 
 * @return La matriz de proyección de la cámara como un `glm::mat4`.
 */
glm::mat4 Camera::GetProjection()
{
	return projection;
}


/**
 * @brief Obtiene la posición actual de la cámara.
 * 
 * @return La posición de la cámara como un `glm::vec3`.
 */
glm::vec3 Camera::GetPosition()
{
	return Position;
}


/**
 * @brief Maneja los eventos de entrada para controlar la cámara.
 * 
 * Ajusta la posición y la configuración de la cámara según las teclas presionadas y el movimiento del ratón.
 * 
 * @param event Evento SDL que contiene información de entrada.
 */
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
			break;

        case SDLK_s:
            Position += speed * -Orientation;
            break;
        case SDLK_d:
            Position += speed * glm::normalize(glm::cross(Orientation, Up));
			break;

		case SDLK_SPACE:
			Position += speed * Up;
			break;

		case SDLK_LCTRL:
			Position += speed * -Up;
			break;

		
		case SDLK_PLUS:
			radius += 0.5f;
			break;

		case SDLK_MINUS:
			radius -= 0.5f;
			break;

        
        default:
            break;
        }
    }

	
	if (event.type == SDL_MOUSEWHEEL )
	{
		if (event.wheel.y > 0) 
			radius -= 0.5f;
		if (event.wheel.y < 0) 
			radius += 0.5f;
	}
	
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_MIDDLE) {
			middleButtonPressed = true;
		}
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_MIDDLE) {
			middleButtonPressed = false;
		}
	} else if (event.type == SDL_MOUSEMOTION && middleButtonPressed) {
		

		/*
		int deltaX = event.motion.xrel;
		int deltaY = event.motion.yrel;
		std::cout << "Mouse Position: (" << deltaX << ", " << deltaY << ")" << std::endl;

		float rotX = sensitivity * (float)(deltaX - (height / 2)) / height;
		float rotY = sensitivity * (float)(deltaX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		*/

		int deltaX = event.motion.xrel;
		int deltaY = event.motion.yrel;
		azimuth += deltaX * sensitivity;
		elevation += deltaY * sensitivity;

		 if (elevation > 89.0f)
        elevation = 89.0f;
   		 if (elevation < -89.0f)
        elevation = -89.0f;

		/*
		const float radPitch{ glm::radians(mPitch) };
		const float radYaw{ glm::radians(mYaw) };
		const glm::vec3 front{
			glm::cos(radYaw) * cos(radPitch),
			glm::sin(radPitch),
			glm::sin(radYaw) * glm::cos(radPitch)
		};
		Orientation = glm::normalize(front);
		mRightDir = glm::normalize(glm::cross(Orientation, Up));
		Up = glm::normalize(glm::cross(mRightDir, Orientation));
		*/

	}




			/*
				if (event.type == SDL_MOUSEMOTION) 
				{
					// Obtener los desplazamientos del mouse
					int deltaX = event.motion.xrel;
					int deltaY = event.motion.yrel;


					float rotX = sensitivity * (float)(deltaY - (height / 2)) / height;
					float rotY = sensitivity * (float)(deltaX - (width / 2)) / width;

					glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

					if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
					{
						Orientation = newOrientation;
					}

					// Rotates the Orientation left and right
					Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
				
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