#pragma once    

#include "common.hpp"

#include "camera/Camera.hpp"
#include "shader/Shader.hpp"
#include "models/models.hpp"
class Scene;

class UI
{
public:
    UI (const unsigned int width, const unsigned int height);

    unsigned int width;
    unsigned int height;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_GLContext glContext = NULL;
    //SDL_GLContext glContext = SDL_GL_CreateContext(window);

    void Update(Scene&);

    void MainMenu(Scene&);
    void NormalsMenu();
    void EditVertexMenu(Model&);
    void HistogramMenu();
    void FixMenu(Model& );
    void PersolizeMenu();

    void Destroy();

    int closestIndex = -1;
    std::vector <Vertex> vertice;
    bool createOctree = false;
    bool editMode = false;


    int qtyHexa = 0, qtyTetra = 0, qtyPyra = 0, qtyPrism = 0;

    std::vector<float> histogramData;
    std::vector<float> histogramLabels;

    std::vector<float> Jdata, JRdata, JENSdata, EQdata;
    std::vector<float> ARtotal, ARdata, ARGdata, ARENdata;
    


    int  binsJ = 10,  binsAR = 10;

    bool showAll = true, showJ = false, showJR = false, showJens = false , showEQ = false;
    bool showAllAR = true, showAR = false, showARG = false, showARen = false;

    bool includeHexa = true, includeTetra = true, includePyra = true, includePrism = true;
    bool includeHexa2 = true, includeTetra2 = true, includePyra2 = true, includePrism2 = true;

    bool eM = false, jM = false, aM = false, fM = false, pM = false; 

    ImGui::FileBrowser fileDialog;


private:


    bool show_normals = false;
    bool show_demo_window = false;
    bool show_another_window = false;

    ImVec4 normals_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float normal_scale = 0.1f;

};
