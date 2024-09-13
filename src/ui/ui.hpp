#pragma once    

#include "common.hpp"


#include "camera/Camera.hpp"
#include "shader/Shader.hpp"
#include "mesh/mesh.hpp"

class Scene;

class UI
{
public:

    // Constructor: Inicilizar la ventana y la interfaz de usuario
    UI (const unsigned int width, const unsigned int height);

    unsigned int width;
    unsigned int height;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_GLContext glContext = NULL;

    void Update(Scene&);
    void MainMenu(Scene&);
    void EditVertexMenu(Scene&);
    void JacobianMenu();
    void RatioMenu();
    void FixMenu(Mesh& );
    void CustomizeMenu();

    void Destroy();

    int closestIndex = -1;
    std::vector <Vertex> vertice;
    bool createOctree = false;
    bool editMode = false;

    const char* filename;
    size_t qtyVertex = 0, qtyPolyhedron = 0;
    int qtyHexa = 0, qtyTetra = 0, qtyPyra = 0, qtyPrism = 0;

    std::vector<float>* JTotalptr;
    std::vector<float>* Jsptr;
    //std::vector<float>* Jrdata;
    std::vector<float>* Jensptr;
    //std::vector<float>* EQdata;
    
    std::vector<float>* ARTotalptr;
    std::vector<float>* ARptr;
    std::vector<float>* ARGptr;
    std::vector<float>* ARenptr;
    


    int  binsJ = 10,  binsAR = 10;

    bool showAll = true, showJ = false, showJR = false, showJens = false , showEQ = false;
    bool showAllAR = true, showAR = false, showARG = false, showARen = false;

    bool includeHexa = true, includeTetra = true, includePyra = true, includePrism = true;
    bool includeHexa2 = true, includeTetra2 = true, includePyra2 = true, includePrism2 = true;

    

private:

    ImGui::FileBrowser fileDialog;

    // Variables para gestionar los menus activos en pantalla.
    bool showEditMenu = false, showJacobianMenu = false, showRatioMenu = false, showFixMenu = false, showCustomizeMenu = false; 
    
    bool show_normals = false;
    bool show_demo_window = false;
    bool show_another_window = false;


};
