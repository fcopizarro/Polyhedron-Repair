

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
    
    scene.LoadObject("vtks/Hexaedro.vtk", glm::vec3(1.0f, 0.0f, 0.0f));
    
    bool quit = false;    

    
    int msaa_buffers;
    int msaa_samples;

    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &msaa_buffers);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &msaa_samples);

    printf("MSAA Buffers: %d\n", msaa_buffers);
    printf("MSAA Samples: %d\n", msaa_samples);

	while (!quit)
	{
        event.handle(quit, *scene.camera, *scene.model, ui);

		shader.Activate();
        
        scene.Update(shader, ui);
        

        /*
        if (ui.createOctree)
        {
            OctreeNode* octree = new OctreeNode(BoundingBox (scene.model->boundary.min, scene.model->boundary.max), 0, 5);
            octree->subdivide(scene.model->get_vertices(), scene.model->get_tris());
            Model octree_model(octree);
            //models_loaded.push_back(octree_model);
            ui.createOctree = false;
        }
        */


        ui.Update(*scene.model);

	}

    shader.Delete();
    ui.Destroy();
	
	return 0;
}