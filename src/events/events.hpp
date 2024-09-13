#pragma once    

#include "common.hpp"
#include "ui/ui.hpp"
#include "scene/scene.hpp"


/**
 * @brief Clase que maneja los eventos de entrada y actualización.
 * 
 * La clase `Event` es responsable de gestionar los eventos de entrada, como la interacción del usuario con la
 * interfaz gráfica y los eventos del ratón. También se encarga de procesar estos eventos para actualizar la escena
 * y la interfaz de usuario según sea necesario.
 */

class Event
{
private:
    /* data */
public:
    void event();
    void handle(bool&, Scene&, UI&);

};
