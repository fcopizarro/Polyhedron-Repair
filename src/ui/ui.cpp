#include "ui.hpp"
#include "scene/scene.hpp"


/**
 * @class UI
 * @brief Clase que representa una interfaz de usuario gráfica.
 *
 * Este constructor inicializa la ventana gráfica utilizando SDL y configura 
 * OpenGL para el renderizado. Activa características como el buffer de profundidad 
 * y el antialiasing (MSAA).
 *
 * @param width: Ancho inicial de la ventana en píxeles.
 * @param height: Altura inicial de la ventana en píxeles.
 */
UI::UI (const unsigned int width, const unsigned int height)
{ 
    // Asignar las dimensiones iniciales de la ventana
    this->width = width;
    this->height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
    }
    
    // Configurar SDL para el uso de MSAA
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) < 0) {
        printf("Error configurando SDL_GL_MULTISAMPLEBUFFERS: %s\n", SDL_GetError());
    }
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4) < 0) {
        printf("Error configurando SDL_GL_MULTISAMPLESAMPLES: %s\n", SDL_GetError());
    }
    

    // Crear la ventana
    window = SDL_CreateWindow("SDL VTK Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Error al crear el contexto OpenGL: " << SDL_GetError() << std::endl;
    }
    
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }


    SDL_GL_SetSwapInterval(0); 
    SDL_GL_MakeCurrent(window, glContext);
    glViewport( 0, 0, width, height );

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    

    glEnable(GL_FLAT);
    glShadeModel(GL_FLAT);

    //glEnable(GL_NORMALIZE);   // Normalizar normales automáticamente
    
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    

}

void UI::Update(Scene& scene) 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    MainMenu(scene);

    NormalsMenu();

    EditVertexMenu(*scene.model);
    FixMenu(*scene.model);

    PersolizeMenu();



    HistogramMenu();

    fileDialog.Display();


    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}



void UI::HistogramMenu()
{

    if (metricsMenu)
    {

    if(ImGui::Begin("Jacobian Metrics", &metricsMenu)) {
        ImGui::SetNextItemWidth(200);
        if (ImGui::RadioButton("N Bins",binsJ>=0))                  { binsJ = 50;}
        if (binsJ>=0) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::SliderInt("##Bins", &binsJ, 1, 100);
        }

        ImGui::Checkbox("Todo", &showAll);
        ImGui::SameLine();
        ImGui::Checkbox("Js", &showJ);
        ImGui::SameLine();
        ImGui::Checkbox("JR", &showJR);
        ImGui::SameLine();
        ImGui::Checkbox("Jens", &showJens);
        //ImGui::SameLine();
        //ImGui::Checkbox("EQ", &showEQ);



        ImGui::Checkbox("Hexahedra", &includeHexa);
        ImGui::SameLine();
        ImGui::Checkbox("Tetrahedra", &includeTetra);
        ImGui::SameLine();
        ImGui::Checkbox("Pyramid", &includePyra);
        ImGui::SameLine();
        ImGui::Checkbox("Prism", &includePrism);

        static bool range = false;
        ImGui::Checkbox("Range", &range);
        static float rmin = -1;
        static float rmax = 1;
        if (range) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::DragFloat2("##Range",&rmin,0.01f,-1,1);
        }



        if (ImPlot::BeginPlot("##Histograms")) {
            ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);

            if(showAll)
            {
            ImPlot::SetNextLineStyle(ImVec4(1,0,0,1));
            ImPlot::SetNextFillStyle(ImVec4(1,0,0,1), 0.25);
            ImPlot::PlotHistogram("Total", histogramData.data(), histogramData.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showJ)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,1,0,1));
            ImPlot::SetNextFillStyle(ImVec4(0,1,0,1), 0.25);
            ImPlot::PlotHistogram("Scalated Jacobian", Jdata.data(), Jdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showJR)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,0,1,1));
            ImPlot::SetNextFillStyle(ImVec4(0,0,1,1), 0.25);
            ImPlot::PlotHistogram("Jacobian Ratio", JRdata.data(), JRdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showJens)
            {
            ImPlot::SetNextLineStyle(ImVec4(1,1,0,1));
            ImPlot::SetNextFillStyle(ImVec4(1,1,0,1), 0.25);
            ImPlot::PlotHistogram("Jens", JENSdata.data(), JENSdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }

            /*
            if(showEQ)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,1,1,1));
            ImPlot::SetNextFillStyle(ImVec4(0,1,1,1), 0.25);
            ImPlot::PlotHistogram("Quality", EQdata.data(), EQdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }
            */
            
            ImPlot::EndPlot();
        }

    }
    ImGui::End();
  }
}


void UI::RatioMenu()
{


    if (ratioMenu)
    {
    if(ImGui::Begin("Aspect Ratio Metrics", &ratioMenu)) {
        ImGui::SetNextItemWidth(200);
        if (ImGui::RadioButton("N Bins",binsAR>=0))                  { binsAR = 50;}
        if (binsAR>=0) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::SliderInt("##Bins", &binsAR, 1, 100);
        }

        ImGui::Checkbox("Todo", &showAllAR);
        ImGui::SameLine();
        ImGui::Checkbox("AR", &showAR);
        ImGui::SameLine();
        ImGui::Checkbox("ARG", &showARG);
        ImGui::SameLine();
        ImGui::Checkbox("ARen", &showARen);

        ImGui::Checkbox("Hexahedra", &includeHexa2);
        ImGui::SameLine();
        ImGui::Checkbox("Tetrahedra", &includeTetra2);
        ImGui::SameLine();
        ImGui::Checkbox("Pyramid", &includePyra2);
        ImGui::SameLine();
        ImGui::Checkbox("Prism", &includePrism2);
        

        static bool range2 = false;
        ImGui::Checkbox("Range", &range2);
        static float rmin2 = -1;
        static float rmax2 = 1;
        if (range2) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::DragFloat2("##Range",&rmin2,0.01f,-1,1);
        }

            if (ImPlot::BeginPlot("##Histograms2")) {
            ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);

            if(showAllAR)
            {
            ImPlot::SetNextLineStyle(ImVec4(1,0,0,1));
            ImPlot::SetNextFillStyle(ImVec4(1,0,0,1), 0.25);
            ImPlot::PlotHistogram("Total", ARtotal.data(), ARtotal.size(), binsAR, 1.0, range2 ? ImPlotRange(rmin2,rmax2) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showAR)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,1,0,1));
            ImPlot::SetNextFillStyle(ImVec4(0,1,0,1), 0.25);
            ImPlot::PlotHistogram("AR", ARdata.data(), ARdata.size(), binsAR, 1.0, range2 ? ImPlotRange(rmin2,rmax2) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showARG)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,0,1,1));
            ImPlot::SetNextFillStyle(ImVec4(0,0,1,1), 0.25);
            ImPlot::PlotHistogram("ARG", ARGdata.data(), ARGdata.size(), binsAR, 1.0, range2 ? ImPlotRange(rmin2,rmax2) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showARen)
            {
            ImPlot::SetNextLineStyle(ImVec4(1,1,0,1));
            ImPlot::SetNextFillStyle(ImVec4(1,1,0,1), 0.25);
            ImPlot::PlotHistogram("ARen", ARENdata.data(), ARENdata.size(), binsAR, 1.0, range2 ? ImPlotRange(rmin2,rmax2) : ImPlotRange(-1.0f, 1.0f));
            }
            
            ImPlot::EndPlot();
            }
        }
    ImGui::End();
    }
}


void UI::MainMenu(Scene& scene)
{
    
    
    static float f = 0.0f;
    static int counter = 0;

    //ImGui::SetNextWindowSize(ImVec2((float)200.0f, (float)400.0f));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    static bool my_tool_active = true;

    ImGui::Begin("Menu Principal", &my_tool_active, ImGuiWindowFlags_MenuBar);                          // Create a window called "Hello, world!" and append into it.

    if (ImGui::BeginMenuBar())
{
    if (ImGui::BeginMenu("Archivo"))
    {
        if (ImGui::MenuItem("Abrir..", "Ctrl+O")) { fileDialog.Open(); }
        if (ImGui::MenuItem("Guardar", "Ctrl+S"))   { /* Do stuff */ }
        if (ImGui::MenuItem("Cerrar", "Ctrl+W"))  
        { 
            if (scene.model_in_scene)
            {
            delete scene.model; //freed memory
            scene.model = NULL;
            scene.model_in_scene = false;

            histogramData.clear();
            Jdata.clear();
            JRdata.clear();
            JENSdata.clear();
            EQdata.clear();

            ARtotal.clear();
            ARdata.clear();
            ARGdata.clear();
            ARENdata.clear();

            //scene.model->lineVertices.clear();
            //scene.model->UpdateMeshLines();
            }
         }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}


    //ImGui::Checkbox("Normales", &show_another_window);

    /*
    static int listbox_item_current = 0;

    std::vector<const char*> cstrings;
    for (const auto& item : models_loaded)
    {
        cstrings.push_back(item.filename.c_str());
    }

    ImGui::ListBox("##listbox", &listbox_item_current, cstrings.data(), static_cast<int>(cstrings.size()), 4);

    cstrings.clear();
    */
    
    /*
    ImGui::ListBox(
                    "listbox:0:Cavebot", 
                    &listbox_item_current, 
                    waypoint_getter, 
                    listbox_items.data(), 
                    listbox_items.size()
                );*/
    
//ImGui::ListBox("Symbols", listbox_item_current, VectorOfStringGetter(, (void*)&models_loaded, (int)models_loaded.size());

    // ImGui::ListBox("", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);



    
    if(fileDialog.HasSelected())
    {
            scene.LoadObject(fileDialog.GetSelected().string(), glm::vec3(1.0f, 0.0f, 0.0f));
            std::cout << "Selected filename " << fileDialog.GetSelected().string() << std::endl;
            fileDialog.ClearSelected();
            //scene.LoadObject("vtks/Prueba3.vtk", glm::vec3(1.0f, 0.0f, 0.0f));

                
            scene.model->polyMesh.CalculateJ();
            scene.model->polyMesh.GetJ();
            histogramData.clear();

            histogramData = scene.model->polyMesh.Jtotal;
            Jdata = scene.model->polyMesh.Jdata;
            JRdata = scene.model->polyMesh.JRdata;
            JENSdata = scene.model->polyMesh.JENSdata;
            EQdata = scene.model->polyMesh.EQdata;

            ARtotal = scene.model->polyMesh.ARtotal;
            ARdata = scene.model->polyMesh.ARdata;
            ARGdata = scene.model->polyMesh.ARGdata;
            ARENdata = scene.model->polyMesh.ARENdata;
    }

   if(scene.model_in_scene)
   {
    ImGui::Text("Malla cargada desde:\n%s", scene.model->filename.c_str());

    ImGui::Text("\nInformacion de la Malla");

    ImGui::Text("\nN° de vertices: %zu", scene.model->vertices.size());

    ImGui::Text("\nN° de poliedros: %zu", scene.model->polyMesh.polys.size());
    ImGui::Text(" %i Hexaedra", qtyHexa);
    ImGui::Text(" %i Tetrahedra", qtyTetra);
    ImGui::Text(" %i Pyramid", qtyPyra);
    ImGui::Text(" %i Prism", qtyPrism);


    ImGui::Text("\n\nMenús");
    ImGui::Checkbox("Edición", &editMenu);
    ImGui::SameLine();
    ImGui::Checkbox("Metricas Jacobianas", &metricsMenu);
    ImGui::Checkbox("Metricas AR", &ratioMenu);
    ImGui::SameLine();
    ImGui::Checkbox("Reparación", &fixMenu);

    ImGui::Text("\n");
    ImGui::Checkbox("Personalización", &customizeMenu);




    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("\n\n%.3f FPS", io.Framerate);
   }


    ImGui::End();
}



void UI::EditVertexMenu(Model& model)
{
    if (editMenu){

    ImGui::SetNextWindowSize(ImVec2((float)300.0f, (float)250.0f));
    ImGui::SetNextWindowPos(ImVec2((float)width - 250.0f - 10.0f , 0));

    ImGui::Begin("Menu Edicion" , &editMenu);                          // Create a window called "Hello, world!" and append into it.
    
    ImGui::Checkbox("Modo Edición", &editMode);

    

    if (editMode)
    {
        
        if (closestIndex != -1)
        {
            ImGui::Text("Vertice seleccionado %i", closestIndex );

            float vec4a[3] = {vertice[closestIndex].position.x, vertice[closestIndex].position.y, vertice[closestIndex].position.z};
            float bef[3] = {vertice[closestIndex].position.x, vertice[closestIndex].position.y, vertice[closestIndex].position.z};
            ImGui::InputFloat3("Posicion x, y , z", vec4a, "%.3f",  ImGuiInputTextFlags_EnterReturnsTrue);

            if (bef[0] != vec4a[0] || bef[1] != vec4a[1] || bef[2] != vec4a[2] )
             {
                model.setVertexPosition(closestIndex, vec4a[0], vec4a[1], vec4a[2]);
                model.updateNormals();
                model.UpdateMeshLines();
                model.polyMesh.CalculateJ();
                model.polyMesh.GetJ();
                histogramData.clear();
                /*
                std::cout << " Cmbiasdiasdfjasdf    ";
                for (auto J_: model.polyMesh.Jtotal)
                {   
                    std::cout << J_ << "  ";
                }
                */
                std::cout << std::endl;
                histogramData = model.polyMesh.Jtotal;
                Jdata = model.polyMesh.Jdata;
                JRdata = model.polyMesh.JRdata;
                JENSdata = model.polyMesh.JENSdata;
                EQdata = model.polyMesh.EQdata;

                ARtotal = model.polyMesh.ARtotal;
                ARdata = model.polyMesh.ARdata;
                ARGdata = model.polyMesh.ARGdata;
                ARENdata = model.polyMesh.ARENdata;
                }

        } else {
            ImGui::Text("No se ha seleccionado un vertice.");
        }
    }

    ImGui::End();
    }
}


void UI::PersolizeMenu()
{
    
    if (customizeMenu)
    {
    ImGui::Begin("Menu Personalización", &customizeMenu);                          // Create a window called "Hello, world!" and append into it.
    
    

    ImVec4 colorFondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Text("Color Fondo");
    ImGui::ColorEdit3("", (float*)&colorFondo);


    ImVec4 colorMalla = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    ImGui::Text("Color Malla");
    ImGui::ColorEdit3("", (float*)&colorMalla);

    bool normales = false;
    ImGui::Checkbox("Dibujar Normales", &normales);

    bool lineas = true;
    ImGui::Checkbox("Dibujar Aristas", &lineas);


    


    ImGui::End();
    }
}

void UI::NormalsMenu()
{
    // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Menu de Normales", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Checkbox("Visualizarlas", &show_normals);
            ImGui::SliderFloat("float", &normal_scale, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&normals_color);

            ImGui::End();
        }

}

void UI::FixMenu(Model& model) 
{
    if (fixMenu)
    {
    static float scaleFix = 0.0f;
    ImGui::Begin("Menu Fix", &fixMenu);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //ImGui::Checkbox("Visualizarlas", &show_normals);
    //ImGui::SliderFloat("float", &normal_scale, 0.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3("clear color", (float*)&normals_color);

    ImGui::Text("Seleccione el metodo de reparacion");
    static int metodo = 0;
    ImGui::RadioButton("Aleatorio", &metodo, 0); 
    ImGui::RadioButton("Gradiante", &metodo, 1);


    ImGui::Text("\nSeleccione la metrica a reparar");
    static int e = 0;
    ImGui::RadioButton("Scaled Jacobian", &e, 0); 
    ImGui::RadioButton("Jacobian Ratio", &e, 1);
    ImGui::RadioButton("Jens", &e, 2);

    ImGui::Text("\nSeleccione la calidad minima");
    ImGui::SliderFloat("##", &scaleFix, -1.0f, 1.0f);

    ImGui::Text("\nIntentos");
    static int intentos = 1000;
    ImGui::InputInt("##2", &intentos, -1, 1);

    ImGui::Text("\n");
    if (ImGui::Button("Reparar Malla"))
    {
        model.polyMesh.FixJ(scaleFix, intentos, metodo, e);    
        model.UpdateModelGraph();
        model.updateNormals();
        model.UpdateMeshLines();

        model.polyMesh.CalculateJ();
        model.polyMesh.GetJ();
        histogramData.clear();

        histogramData = model.polyMesh.Jtotal;
        Jdata = model.polyMesh.Jdata;
        JRdata = model.polyMesh.JRdata;
        JENSdata = model.polyMesh.JENSdata;
        EQdata = model.polyMesh.EQdata;

        ARtotal = model.polyMesh.ARtotal;
        ARdata = model.polyMesh.ARdata;
        ARGdata = model.polyMesh.ARGdata;
        ARENdata = model.polyMesh.ARENdata;
    }    

    static int generados = 0;

     if (ImGui::Button("Generar Resultados"))
    {

        std::string filename = "prueba" + std::to_string(generados) + ".txt";
        std::ofstream outfile (filename);

        outfile << "Resumen Js" << std::endl;
        for (auto poly: model.polyMesh.polys)
        {
            for (float J_: poly->J)
                outfile << std::to_string(J_) << std::endl;
        }

        outfile << std::endl << std::endl << "Resumen JR" << std::endl;
        for (auto poly: model.polyMesh.polys)
        {
            for (float J_: poly->JR)
                outfile << std::to_string(J_) << std::endl;
        }

        outfile << std::endl << std::endl << "Resumen Jens" << std::endl;
        for (auto poly: model.polyMesh.polys)
        {
            for (float J_: poly->Jens)
                outfile << std::to_string(J_) << std::endl;
        }

        outfile.close();
        

        generados ++;
    }    
    ImGui::End();
    }

}

void UI::Destroy()
{
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

