#include "ui.hpp"

#include <math.h>


double RandomGauss() {
	static double V1, V2, S;
	static int phase = 0;
	double X;
	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);
	phase = 1 - phase;
	return X;
}

template <int N>
struct NormalDistribution {
    NormalDistribution(double mean, double sd) {
        for (int i = 0; i < N; ++i)
            Data[i] = RandomGauss()*sd + mean;
    }
    double Data[N];
};

void Demo_Histogram() {

    static ImPlotHistogramFlags hist_flags = ImPlotHistogramFlags_Density;
    static int  bins       = 50;
    static double mu       = 5;
    static double sigma    = 2;
    ImGui::SetNextItemWidth(200);
    if (ImGui::RadioButton("Sqrt",bins==ImPlotBin_Sqrt))       { bins = ImPlotBin_Sqrt;    } ImGui::SameLine();
    if (ImGui::RadioButton("Sturges",bins==ImPlotBin_Sturges)) { bins = ImPlotBin_Sturges; } ImGui::SameLine();
    if (ImGui::RadioButton("Rice",bins==ImPlotBin_Rice))       { bins = ImPlotBin_Rice;    } ImGui::SameLine();
    if (ImGui::RadioButton("Scott",bins==ImPlotBin_Scott))     { bins = ImPlotBin_Scott;   } ImGui::SameLine();
    if (ImGui::RadioButton("N Bins",bins>=0))                  { bins = 50;                }
    if (bins>=0) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##Bins", &bins, 1, 100);
    }
    ImGui::CheckboxFlags("Horizontal", (unsigned int*)&hist_flags, ImPlotHistogramFlags_Horizontal);
    ImGui::SameLine();
    ImGui::CheckboxFlags("Density", (unsigned int*)&hist_flags, ImPlotHistogramFlags_Density);
    ImGui::SameLine();
    ImGui::CheckboxFlags("Cumulative", (unsigned int*)&hist_flags, ImPlotHistogramFlags_Cumulative);

    static bool range = false;
    ImGui::Checkbox("Range", &range);
    static float rmin = -3;
    static float rmax = 13;
    if (range) {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::DragFloat2("##Range",&rmin,0.1f,-3,13);
        ImGui::SameLine();
        ImGui::CheckboxFlags("Exclude Outliers", (unsigned int*)&hist_flags, ImPlotHistogramFlags_NoOutliers);
    }
    static NormalDistribution<10000> dist(mu, sigma);
    static double x[100];
    static double y[100];
    if (hist_flags & ImPlotHistogramFlags_Density) {
        for (int i = 0; i < 100; ++i) {
            x[i] = -3 + 16 * (double)i/99.0;
            y[i] = exp( - (x[i]-mu)*(x[i]-mu) / (2*sigma*sigma)) / (sigma * sqrt(2*3.141592653589793238));
        }
        if (hist_flags & ImPlotHistogramFlags_Cumulative) {
            for (int i = 1; i < 100; ++i)
                y[i] += y[i-1];
            for (int i = 0; i < 100; ++i)
                y[i] /= y[99];
        }
    }

    if (ImPlot::BeginPlot("##Histograms")) {
        ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        ImPlot::PlotHistogram("Empirical", dist.Data, 10000, bins, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(), hist_flags);
        if ((hist_flags & ImPlotHistogramFlags_Density) && !(hist_flags & ImPlotHistogramFlags_NoOutliers)) {
            if (hist_flags & ImPlotHistogramFlags_Horizontal)
                ImPlot::PlotLine("Theoretical",y,x,100);
            else
                ImPlot::PlotLine("Theoretical",x,y,100);
        }
        ImPlot::EndPlot();
    }

}



void UI::HistogramMenu()
{


    if(ImGui::Begin("Jacobian Metrics")) {
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
        ImGui::SameLine();
        ImGui::Checkbox("EQ", &showEQ);



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

            if(showEQ)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,1,1,1));
            ImPlot::SetNextFillStyle(ImVec4(0,1,1,1), 0.25);
            ImPlot::PlotHistogram("Quality", EQdata.data(), EQdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }
            
            ImPlot::EndPlot();
        }

    }
    ImGui::End();

    
    if(ImGui::Begin("Aspect Ratio Metrics")) {
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

UI::UI (const unsigned int width,const unsigned int height)
{
    this->width = width;
    this->height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
    }
    
    // Configurar SDL para utilizar MSAA
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
    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    

    glEnable(GL_FLAT);
    glShadeModel(GL_FLAT);

    glEnable(GL_NORMALIZE);   // Normalizar normales automáticamente
    
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

}

void UI::Update(Model& model) 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    MainMenu();

    NormalsMenu();

    EditVertexMenu(model);



    HistogramMenu();

    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void UI::MainMenu()
{
    
    static float f = 0.0f;
    static int counter = 0;

    ImGui::SetNextWindowSize(ImVec2((float)300.0f, (float)400.0f));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Menu Principal");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("Meshes cargados");               // Display some text (you can use a format strings too)

    /*
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    */
   ImGuiIO& io = ImGui::GetIO();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    

    ImGui::Checkbox("Normales", &show_another_window);

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


    ImGui::Button("Reemplazar");
    ImGui::SameLine();
    ImGui::Button("Agregar");

    if (ImGui::Button("Crear Octree"))
    {
        std::cout << "Creando Octree" << std::endl;
        createOctree = true;

        /*
        OctreeNode* octree = new OctreeNode(BoundingBox (models_loaded[0].boundary.min, models_loaded[0].boundary.max), 0, 4);
        octree->subdivide(models_loaded[0].get_vertices());
        Model octree_model(octree);
        models_loaded.push_back(octree_model);
        models_loaded[0].display = false;
        */

    }

    ImGui::Text("%i Hexaedra", qtyHexa);
    ImGui::Text("%i Tetrahedra", qtyTetra);
    ImGui::Text("%i Pyramid", qtyPyra);
    ImGui::Text("%i Prism", qtyPrism);


    ImGui::End();
}



void UI::EditVertexMenu(Model& model)
{
    
    ImGui::SetNextWindowSize(ImVec2((float)300.0f, (float)250.0f));
    ImGui::SetNextWindowPos(ImVec2((float)width - 250.0f - 10.0f , 0));

    ImGui::Begin("Menu Edicion");                          // Create a window called "Hello, world!" and append into it.
    
    ImGui::Checkbox("Modo Edición", &editMode);

    

    if (editMode)
    {
        
        if (closestIndex != -1)
        {
            ImGui::Text("Vertice seleccionado");

            float vec4a[3] = {vertice[closestIndex]->position.x, vertice[closestIndex]->position.y, vertice[closestIndex]->position.z};
            float bef[3] = {vertice[closestIndex]->position.x, vertice[closestIndex]->position.y, vertice[closestIndex]->position.z};
            ImGui::InputFloat3("Posicion x, y , z", vec4a);



            glm::vec3 tempvec = model.get_vertices()[closestIndex]->position;
            ImGui::Text("Version Model %.2f %.2f %.2f", tempvec.x, tempvec.y, tempvec.z);
            ImGui::Text("Version UI %.2f %.2f %.2f", vertice[closestIndex]->position.x, vertice[closestIndex]->position.y, vertice[closestIndex]->position.z);
            ImGui::Text("Version Poly %.2f %.2f %.2f", model.polyMesh.vertexs[closestIndex].x, model.polyMesh.vertexs[closestIndex].y, model.polyMesh.vertexs[closestIndex].z);



            if (bef[0] != vec4a[0] || bef[1] != vec4a[1] || bef[2] != vec4a[2] )
                {
                model.setVertexPosition(closestIndex, vec4a[0], vec4a[1], vec4a[2]);
                model.polyMesh.CalculateJ();
                model.polyMesh.GetJ();
                histogramData.clear();
                std::cout << " Cmbiasdiasdfjasdf    ";
                for (auto J_: model.polyMesh.Jtotal)
                {   
                    std::cout << J_ << "  ";
                }
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

