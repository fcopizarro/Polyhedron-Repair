#include "common.hpp"

#include "ui/ui.hpp"
#include "events/events.hpp"
#include "scene/scene.hpp"

#include <string>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


int main(int argc, char* argv[])
{
    const unsigned int width = 1000;
    const unsigned int height = 1000;
    
    UI ui(width, height);
    Scene scene(width, height);

    Shader shader("../src/shader/default/vertex.glsl", "../src/shader/default/fragment.glsl");
    Event event;
    
    
    bool quit = false;    

    
    int msaa_buffers;
    int msaa_samples;

    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &msaa_buffers);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &msaa_samples);

    printf("MSAA Buffers: %d\n", msaa_buffers);
    printf("MSAA Samples: %d\n", msaa_samples);

	while (!quit)
	{
        glClearColor(0.07f, 0.13f, 0.17f, 2.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        event.handle(quit, scene, ui);

	shader.Activate();
        
        scene.Update(shader, ui);

        ui.Update(scene);

	}

    shader.Delete();
    ui.Destroy();
	
	return 0;
}
