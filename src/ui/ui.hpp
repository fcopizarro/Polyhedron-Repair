#pragma once    

#include "common.hpp"

#include "camera/Camera.hpp"
#include "shader/Shader.hpp"
#include "mesh/mesh.hpp"
#include <math.h>

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
    void EditVertexMenu(Mesh&);
    void JacobianMenu();
    void RatioMenu();
    void FixMenu(Mesh& );
    void CustomizeMenu();

    void Destroy();

    int closestIndex = -1;
    std::vector <Vertex> vertice;
    bool createOctree = false;
    bool editMode = false;


    int qtyHexa = 0, qtyTetra = 0, qtyPyra = 0, qtyPrism = 0;

    std::vector<float> JTotal;
    std::vector<float> histogramLabels;

    std::vector<float> Jdata, JRdata, JENSdata, EQdata;
    std::vector<float> ARtotal, ARdata, ARGdata, ARENdata;
    


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

    ImVec4 normals_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float normal_scale = 0.1f;

};
