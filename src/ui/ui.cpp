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

    // Intentar iniciar la ventana de SDL2 e imprimir en caso de error
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error inicializando SDL: %s\n", SDL_GetError());
    }
    
    // Configurar SDL para el uso de MSAA
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) < 0)
        printf("Error configurando SDL_GL_MULTISAMPLEBUFFERS: %s\n", SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2) < 0) 
        printf("Error configurando SDL_GL_MULTISAMPLESAMPLES: %s\n", SDL_GetError());

    // Crear la ventana con  SDL
    window = SDL_CreateWindow("Polyhedron Repair", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
    }
  
    // Crear un contexto de OpenGL asociado a la ventana SDL.
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Error al crear el contexto OpenGL: " << SDL_GetError() << std::endl;
    }
    
    // Habilitar e Inicializar la extensión GLEW para asegurar la compatibilidad con nuevas extensiones de OpenGL.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    // Intervalo de intercambio de buffers de OpenGL a 0
    SDL_GL_SetSwapInterval(0);
    
    // Vincular el actual contexto de OpenGL con la ventana creada 
    SDL_GL_MakeCurrent(window, glContext);
    
    // Configura el área de visualización de OpenGL a tamaño completo.
    glViewport( 0, 0, width, height );
  
    // Inicializar ImGui y Implot 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Habilitar uso del teclado
    ImGui::StyleColorsDark(); // Tema Oscuro de la interfaz
    ImGui_ImplSDL2_InitForOpenGL(window, glContext); // Inicializar en el backend SDL2 - OpenGl
    ImGui_ImplOpenGL3_Init("#version 150"); 

    // Habilita el muestreo múltiple (multisampling) para suavizar los bordes 
    glEnable(GL_MULTISAMPLE);
    // Habilita la prueba de profundidad, que asegura que los píxeles más cercanos a la cámara se rendericen sobre
    // los píxeles más lejanos.
    glEnable(GL_DEPTH_TEST);
    
    // Configura el modelo de sombreado a plano.
    glEnable(GL_FLAT);
    glShadeModel(GL_FLAT);
    
    // Definir la normal para un primitivo de el primer vertice que lo compone. 
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    
}


/**
 * @brief Finaliza y libera todos los recursos utilizados por la interfaz de usuario (UI), los motores graficos (OpenGl) y el gestor de ventanas (SDL).
 * 
 * Este método realiza las siguientes tareas de limpieza:
 * 1. Cierra el contexto de OpenGL y libera los recursos asociados a la integración de ImGui con OpenGL.
 * 2. Finaliza la integración de ImGui con SDL2.
 * 3. Destruye el contexto de ImGui.
 * 4. Destruye el contexto de ImPlot.
 * 5. Elimina el contexto de OpenGL asociado a la ventana.
 * 6. Destruye la ventana SDL2.
 * 7. Finaliza la biblioteca SDL.
 * 
 * Asegúrate de que esta función sea llamada antes de cerrar la aplicación para evitar pérdidas de memoria
 * y liberar adecuadamente todos los recursos utilizados.
 */
void UI::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



/**
 * @brief Actualiza y renderiza la interfaz de usuario utilizando ImGui y OpenGL.
 * 
 * Esta función gestiona la actualización de la interfaz gráfica de usuario (UI) en cada frame, 
 * y se encarga de procesar la entrada y renderizar los menús y otros elementos gráficos.
 * 
 * @param scene Referencia al objeto `Scene` que contiene los datos y mallas necesarios para 
 *              actualizar la UI segun lo que esté pasando en el escenario.
 */
void UI::Update(Scene& scene) 
{
    // Iniciar un nuevo frame de ImGui, sincronizandolo con SDL2 y OpenGL.
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Llamada a los menus que componen la interfaz del usuario.
    MainMenu(scene); // Menu Principal
    if(showEditMenu)        EditVertexMenu(*scene.mesh);
    if(showFixMenu)         FixMenu(*scene.mesh);
    if(showCustomizeMenu)   CustomizeMenu();
    if(showJacobianMenu)    JacobianMenu();
    if(showRatioMenu)       RatioMenu();
    
    // Mostrar FileDialog si se ha solicitado Leer o Guardar archivo.
    fileDialog.Display(); 

    // Renderizar frame con toda la informacion actualizada.
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}



/**
 * @brief Muestra el menú principal de la interfaz.
 * 
 * Esta función crea una ventana de ImGui que sirve como menú principal para la aplicación. 
 * Permite al usuario abrir y guardar archivos, cerrar el modelo cargado, mostrar información 
 * sobre la malla cargada, y controlar la visibilidad de diferentes menús y métricas.
 * 
 * @param scene Referencia al objeto `Scene` que contiene datos sobre el modelo y la malla actual.
 */
void UI::MainMenu(Scene& scene)
{
    // Posicionar la ventana en la esquina superior izquierda.
    ImGui::SetNextWindowPos(ImVec2(0, 0));
  
    // Iniciar Menu Principal, con un MenuBar superior para interactuar con los archivos.
    static bool always_active = true;
    ImGui::Begin("Menu Principal", &always_active, ImGuiWindowFlags_MenuBar);  

    // Definir Elementos del MenuBar
    if (ImGui::BeginMenuBar())
    {
        // De momento solo esta desponible el menu "Archivo" para interactuar con los archivos
        if (ImGui::BeginMenu("Archivo"))
        {
            // Abrir el fileDialog y permitir al usuario seleccionar un archivo.
            if (ImGui::MenuItem("Abrir..", "")) { fileDialog.Open(); }
            
            // Abrir el fileDialog y permitir al usuario guardar el archivo en la ubicacion seleccionada.
            if (ImGui::MenuItem("Guardar", ""))   { /* Do stuff */ }
            
            // Eliminar la malla actual y limpiar todo lo almacenado.
            if (ImGui::MenuItem("Cerrar", ""))  
            { 
                if (scene.mesh_in_scene)
                {
                  // Solo eliminar si es que existe una malla cargada
                  
                  // Eliminar modelo e inhabilitar punteros
                  delete scene.mesh;
                  scene.mesh = NULL;
                  scene.mesh_in_scene = false;

                  // Limpiar metricas almacenadas
                  JTotal.clear();
                  Jdata.clear();
                  JRdata.clear();
                  JENSdata.clear();
                  EQdata.clear();
                  ARtotal.clear();
                  ARdata.clear();
                  ARGdata.clear();
                  ARENdata.clear();

                }
             }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }



    // Retorno del fileDilaog, una vez el usuario selecciono un archivo a cargar
    if(fileDialog.HasSelected())
    {
            // Cargar el objeto en la escena.
            scene.LoadObject(fileDialog.GetSelected().string());
            std::cout << "Selected filename " << fileDialog.GetSelected().string() << std::endl;
            
            // Limpiar seleccion
            fileDialog.ClearSelected();

            // Calcular Metricas de la malla cargada
            scene.mesh->polyMesh.CalculateJ();
            //scene.mesh->polyMesh.GetJ();

            // Tomar las metricas calculadas y cargarlas en la interfaz usuaria.
            JTotal = scene.mesh->polyMesh.Jtotal;
            Jdata = scene.mesh->polyMesh.Jdata;
            JRdata = scene.mesh->polyMesh.JRdata;
            JENSdata = scene.mesh->polyMesh.JENSdata;
            EQdata = scene.mesh->polyMesh.EQdata;

            ARtotal = scene.mesh->polyMesh.ARtotal;
            ARdata = scene.mesh->polyMesh.ARdata;
            ARGdata = scene.mesh->polyMesh.ARGdata;
            ARENdata = scene.mesh->polyMesh.ARENdata;
    }

    // Si se ha cargado la malla correctamente
    // Mostrar en el menu la  informacion resumida de la malla cargada.
    if(scene.mesh_in_scene)
    {
      ImGui::Text("Malla cargada desde:\n%s", scene.mesh->filename.c_str());

      ImGui::Text("\nInformacion de la Malla");
      
      // TODO: CREAR METODO PARA EXTRAER INFORMACION
      ImGui::Text("\nN° de vertices: %zu", scene.mesh->vertices.size());

      ImGui::Text("\nN° de poliedros: %zu", scene.mesh->polyMesh.polyhedrons.size());
      ImGui::Text(" %i Hexaedra", qtyHexa);
      ImGui::Text(" %i Tetrahedra", qtyTetra);
      ImGui::Text(" %i Pyramid", qtyPyra);
      ImGui::Text(" %i Prism", qtyPrism);


      ImGui::Text("\n\nMenús");
      
      ImGui::Checkbox("Edición", &showEditMenu);
      ImGui::SameLine();
      ImGui::Checkbox("Metricas Jacobianas", &showJacobianMenu);
      
      ImGui::Checkbox("Metricas AR", &showRatioMenu);
      ImGui::SameLine();
      ImGui::Checkbox("Reparación", &showFixMenu);

      ImGui::Text("\n");
      ImGui::Checkbox("Personalización", &showCustomizeMenu);

      // Mostrar FPS
      ImGuiIO& io = ImGui::GetIO();
      ImGui::Text("\n\n%.3f FPS", io.Framerate);
    }


    ImGui::End();
}

/**
 * @brief Muestra un menú para visualizar un histograma y ajustar las métricas basadas en el Jacobiano.
 * 
 * Este método crea una ventana de ImGui que permite al usuario ajustar parámetros para visualizar 
 * histogramas de datos relacionados con métricas de Jacobiano, incluyendo opciones para seleccionar 
 * qué datos mostrar y cómo configurar el rango de los histogramas.
 */
void UI::JacobianMenu()
{
    // Inicia una ventana con el título "Jacobian Metrics".
    if(ImGui::Begin("Jacobian Metrics", &showJacobianMenu)) {
    
        // Establece el ancho de la barra deslizante a  200 píxeles.
        ImGui::SetNextItemWidth(200);
        
        // Deslizador que permite seleccionar el numero de barras del histograma, se definen inicialmente 50.
        if (ImGui::RadioButton("N Bins",binsJ>=0))                  { binsJ = 50;}
        if (binsJ>=0) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::SliderInt("##Bins", &binsJ, 1, 100);
        }
        
        // Casillas de verificación para mostrar diferentes tipos de metricas, seleccionadas por el usuario.
        ImGui::Checkbox("Todo", &showAll);
        ImGui::SameLine();
        ImGui::Checkbox("Js", &showJ);
        ImGui::SameLine();
        ImGui::Checkbox("Jens", &showJens);

        // Casillas de verificación para filtrar las metricas por tipo de poliedro seleccionado.
        ImGui::Checkbox("Hexahedra", &includeHexa);
        ImGui::SameLine();
        ImGui::Checkbox("Tetrahedra", &includeTetra);
        ImGui::SameLine();
        ImGui::Checkbox("Pyramid", &includePyra);
        ImGui::SameLine();
        ImGui::Checkbox("Prism", &includePrism);
  
        // Deslizador para habilitar el ajuste de rango en los histogramas.
        // Inicialmente el rango esta definido entre -1 y 1, con saltos de 0.01.
        static bool range = false;
        ImGui::Checkbox("Range", &range);
        static float rmin = -1;
        static float rmax = 1;
        if (range) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::DragFloat2("##Range",&rmin,0.01f,-1,1);
        }


        // Inicia un nuevo gráfico en ImPlot para mostrar histogramas de datos relacionados con el Jacobiano.
        if (ImPlot::BeginPlot("##Histograms")) {
            // Configura los ejes del gráfico para ajustar automáticamente su tamaño.
            ImPlot::SetupAxes(nullptr,nullptr,ImPlotAxisFlags_AutoFit,ImPlotAxisFlags_AutoFit);
  
            // Dibuja histogramas basados en los datos seleccionados por el usuario, con diferentes colores y estilos.
            if(showAll)
            {
              // Definir colores de las barras
              ImPlot::SetNextLineStyle(ImVec4(1,0,0,1));
              ImPlot::SetNextFillStyle(ImVec4(1,0,0,1), 0.25);
              // Dar los datos que son transformados al histograma
              ImPlot::PlotHistogram("Total", JTotal.data(), JTotal.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }

            if(showJ)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,1,0,1));
            ImPlot::SetNextFillStyle(ImVec4(0,1,0,1), 0.25);
            ImPlot::PlotHistogram("Scaled Jacobian", Jdata.data(), Jdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }
            
            // TODO: Deshabilitado por necesitar ajustes en el calculo del jacobianao
            /*
            if(showJR)
            {
            ImPlot::SetNextLineStyle(ImVec4(0,0,1,1));
            ImPlot::SetNextFillStyle(ImVec4(0,0,1,1), 0.25);
            ImPlot::PlotHistogram("Jacobian Ratio", JRdata.data(), JRdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }
            */

            if(showJens)
            {
            ImPlot::SetNextLineStyle(ImVec4(1,1,0,1));
            ImPlot::SetNextFillStyle(ImVec4(1,1,0,1), 0.25);
            ImPlot::PlotHistogram("Jens", JENSdata.data(), JENSdata.size(), binsJ, 1.0, range ? ImPlotRange(rmin,rmax) : ImPlotRange(-1.0f, 1.0f));
            }
            
            // Finaliza el gráfico,
            ImPlot::EndPlot();
        }
    }
    
    // Finaliza la ventana de ImGui.
    ImGui::End();
}

/**
 * @brief Muestra un menú para visualizar un histograma y ajustar las métricas basadas en la Relacion de Aspecto.
 * 
 * Este método crea una ventana de ImGui que permite al usuario ajustar parámetros para visualizar 
 * histogramas de datos relacionados con metricas basadas en la Relacion de Aspecto de algun poliedro, incluyendo opciones para seleccionar 
 * qué datos mostrar y cómo configurar el rango de los histogramas.
 */
void UI::RatioMenu()
{
    if(ImGui::Begin("Aspect Ratio Metrics", &showRatioMenu)) {
        ImGui::SetNextItemWidth(200);
        if (ImGui::RadioButton("N Bins",binsAR>=0))                  { binsAR = 50;}
        if (binsAR>=0) {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(200);
            ImGui::SliderInt("##Bins2", &binsAR, 1, 100);
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


/**
 * @brief Muestra el menú de edición de vértices.
 * 
 * Esta función crea una ventana de ImGui que permite al usuario editar la posición de un vértice seleccionado 
 * en el modelo 3D. El menú de edición solo se muestra si la variable `showEditMenu` es verdadera. 
 * Permite cambiar la posición del vértice seleccionado y actualiza los datos del modelo y los histogramas correspondientes.
 * 
 * @param model Referencia al objeto `Mesh` que contiene la malla 3D que se está editando.
 */
void UI::EditVertexMenu(Mesh& mesh)
{


    ImGui::SetNextWindowSize(ImVec2((float)300.0f, (float)250.0f));
    ImGui::SetNextWindowPos(ImVec2((float)width - 250.0f - 10.0f , 0));

    ImGui::Begin("Menu Edicion" , &showEditMenu);                          // Create a window called "Hello, world!" and append into it.
    
    ImGui::Checkbox("Modo Edición", &editMode);

  
    // En caso de que se este en el modo de edicion
    // Cambiando la forma en que se grafican las lineas
    if (editMode)
    {
        // Verificar si se ha seleccionado algun vertice valido.
        if (closestIndex != -1)
        {
            ImGui::Text("Vertice seleccionado %i", closestIndex );

            // Variable para almacenar la posicion del vertice seleccionado en la interfaz.
            static float vec4a[3] = {vertice[closestIndex].position.x, vertice[closestIndex].position.y, vertice[closestIndex].position.z};
            float actual_position[3] = {vertice[closestIndex].position.x, vertice[closestIndex].position.y, vertice[closestIndex].position.z};
            
            if (actual_position[0] != vec4a[0] || actual_position[1] != vec4a[1] || actual_position[2] != vec4a[2])
            {
              vec4a[0] = actual_position[0];
              vec4a[1] = actual_position[1];
              vec4a[2] = actual_position[2];
            }
            
            // Input con las coordenadas del vertice seleccionado, permitiendolas cambiar desde el menu.
            if(ImGui::InputFloat3("Posicion x, y , z", vec4a, "%.3f"))
            {
                // Identificar si se realizo un cambio en el input, actualizando las metricas necesarios y la malla.
                
                
                // Cambiar posicion en la malla del vertice cambiado.
                mesh.setVertexPosition(closestIndex, vec4a[0], vec4a[1], vec4a[2]);
                
                // Actualizar Grafica de la malla
                mesh.updateNormals();
                mesh.UpdateMeshLines();
                
                // Actualizar Metricas de los poliedros
                mesh.polyMesh.CalculateJ();
                //mesh.polyMesh.GetJ();
                
                // Copiar metricas actualizadas de los poliedros
                JTotal = mesh.polyMesh.Jtotal;
                Jdata = mesh.polyMesh.Jdata;
                //JRdata = mesh.polyMesh.JRdata; // DESHABILITADA
                JENSdata = mesh.polyMesh.JENSdata;
                EQdata = mesh.polyMesh.EQdata;
                ARtotal = mesh.polyMesh.ARtotal;
                ARdata = mesh.polyMesh.ARdata;
                ARGdata = mesh.polyMesh.ARGdata;
                ARENdata = mesh.polyMesh.ARENdata;
                
                
                // TODO: Vincular metricas sin necesidad de almacenarlas
                // TODO: Mostrar metricas del vertice seleccionado
                

            }


        } else {
            ImGui::Text("No se ha seleccionado un vertice.");
        }
    }

    ImGui::End();
}

/**
 * @brief Muestra el menú de personalización de la interfaz.
 * 
 * Esta función crea una ventana de ImGui que permite al usuario ajustar la apariencia visual de la interfaz 
 * de usuario, incluyendo el color de fondo, el color de la malla, y las opciones para dibujar normales y aristas.
 * 
 * - **Color Fondo**: Permite al usuario seleccionar un color para el fondo de la interfaz.
 * - **Color Malla**: Permite al usuario seleccionar un color para la malla.
 * - **Dibujar Normales**: Activa o desactiva la visualización de las normales de la malla.
 * - **Dibujar Aristas**: Activa o desactiva la visualización de las aristas de la malla.
 * 
 * La ventana del menú de personalización se controla mediante la variable `showCustomizeMenu`.
 */
void UI::CustomizeMenu()
{
  
    ImGui::Begin("Menu Personalización", &showCustomizeMenu);                          // Crear ventana 

    // Define el color de fondo predeterminado.
    ImVec4 colorFondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Muestra un texto y un selector de color para el color de fondo.
    ImGui::Text("Color Fondo");
    ImGui::ColorEdit3("", (float*)&colorFondo);

    // Define el color de la malla predeterminado.
    ImVec4 colorMalla = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    
    // Muestra un texto y un selector de color para el color de la malla.
    ImGui::Text("Color Malla");
    ImGui::ColorEdit3("", (float*)&colorMalla);
    
    // Muestra una casilla de verificación para activar o desactivar la visualización de las normales.
    bool normales = false;
    ImGui::Checkbox("Dibujar Normales", &normales);
    
    // Muestra una casilla de verificación para activar o desactivar la visualización de las aristas.
    bool lineas = true;
    ImGui::Checkbox("Dibujar Aristas", &lineas);
    
    // TODO: Permitir dibujar normales y aristas.
    // TODO: Permitir cambiar resolucion
    // TODO: Permitir cambiar MSAA


    ImGui::End();
}


/**
 * @brief Muestra el menú de reparación de la malla en la interfaz de usuario.
 * 
 * Esta función crea una ventana de ImGui titulada "Menu Fix" que permite al usuario seleccionar y aplicar
 * diferentes métodos y métricas para reparar una malla. También ofrece opciones para ajustar la calidad mínima,
 * definir el número de intentos y generar un archivo con los resultados de la reparación.
 * 
 * La función permite:
 * - Seleccionar el método de reparación (Aleatorio o Gradiante).
 * - Seleccionar la métrica a reparar (Scaled Jacobian, Jacobian Ratio, Jens).
 * - Ajustar la calidad mínima de la reparación mediante un deslizador.
 * - Definir el número de intentos para la reparación.
 * - Aplicar la reparación a la malla.
 * - Generar un archivo con los resultados de la reparación.
 * 
 * Variables:
 * - `scaleFix`: Ajusta la calidad mínima de la reparación. Valor inicial es 0.0f.
 * - `metodo`: Selecciona el método de reparación. `0` para Aleatorio y `1` para Gradiante.
 * - `e`: Selecciona la métrica a reparar. `0` para Scaled Jacobian, `1` para Jacobian Ratio, `2` para Jens.
 * - `intentos`: Define el número de intentos para la reparación. Valor inicial es 1000.
 * - `generados`: Contador para generar nombres de archivo únicos para los resultados.
 * 
 * @param mesh Objeto de la clase `Mesh` que representa el modelo de la malla a reparar.
 */
void UI::FixMenu(Mesh& mesh) 
{
    static float scaleFix = 0.0f;
    ImGui::Begin("Menu Fix", &showFixMenu);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

    ImGui::Text("Seleccione el metodo de reparacion");
    static int metodo = 0;
    ImGui::RadioButton("Aleatorio", &metodo, 0); 
    ImGui::RadioButton("Gradiante", &metodo, 1);


    ImGui::Text("\nSeleccione la metrica a reparar");
    static int e = 0;
    ImGui::RadioButton("Scaled Jacobian", &e, 0); 
    //ImGui::RadioButton("Jacobian Ratio", &e, 1);
    ImGui::RadioButton("Jens", &e, 2);

    ImGui::Text("\nSeleccione la calidad minima");
    ImGui::SliderFloat("##", &scaleFix, -1.0f, 1.0f);

    ImGui::Text("\nIntentos");
    static int intentos = 1000;
    ImGui::InputInt("##2", &intentos, -1, 1);

    ImGui::Text("\n");
    if (ImGui::Button("Reparar Malla"))
    {
        mesh.polyMesh.FixJ(scaleFix, intentos, metodo, e);    
        mesh.UpdateModelGraph();
        mesh.updateNormals();
        mesh.UpdateMeshLines();

        mesh.polyMesh.CalculateJ();
        JTotal.clear();

        JTotal = mesh.polyMesh.Jtotal;
        Jdata = mesh.polyMesh.Jdata;
        JRdata = mesh.polyMesh.JRdata;
        JENSdata = mesh.polyMesh.JENSdata;
        EQdata = mesh.polyMesh.EQdata;

        ARtotal = mesh.polyMesh.ARtotal;
        ARdata = mesh.polyMesh.ARdata;
        ARGdata = mesh.polyMesh.ARGdata;
        ARENdata = mesh.polyMesh.ARENdata;
    }    

    static int generados = 0;

     if (ImGui::Button("Generar Resultados"))
    {

        std::string filename = "prueba" + std::to_string(generados) + ".txt";
        std::ofstream outfile (filename);

        outfile << "Resumen Js" << std::endl;
        for (auto poly: mesh.polyMesh.polyhedrons)
        {
            for (float J_: poly->J)
                outfile << std::to_string(J_) << std::endl;
        }

        outfile << std::endl << std::endl << "Resumen JR" << std::endl;
        for (auto poly: mesh.polyMesh.polyhedrons)
        {
            for (float J_: poly->JR)
                outfile << std::to_string(J_) << std::endl;
        }

        outfile << std::endl << std::endl << "Resumen Jens" << std::endl;
        for (auto poly: mesh.polyMesh.polyhedrons)
        {
            for (float J_: poly->Jens)
                outfile << std::to_string(J_) << std::endl;
        }

        outfile.close();
        

        generados ++;
    }    
    ImGui::End();

}

