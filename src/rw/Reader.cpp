#include "Reader.hpp"

Reader::Reader()
{
}

void Reader::read_file(const std::string filename)
{

    std::string model_ext = getExtension(filename);

    
    if (model_ext == "obj")
    {
        read_obj(filename);
        read_status = true;
        
    } else if (model_ext == "vtk")
    {
        read_vtk(filename);
        read_status = true;
    }
    else
    {
        std::cout << "Formato no soportado " << model_ext << std::endl;
        read_status = false;
    }
    
}


/**
 * @brief Verifica si una línea de texto está vacía.
 * 
 * Esta función determina si una línea de texto está vacía o contiene solo caracteres de espacio en blanco, tabulaciones,
 * retornos de carro (CR) y saltos de línea (LF). Es compatible con los diferentes formatos de salto de línea utilizados en Unix
 * y Windows.
 * 
 * Se consideran caracteres en blanco los siguientes:
 * - Espacios (espacio en blanco)
 * - Tabulaciones (tab)
 * - Retornos de carro (CR)
 * - Saltos de línea (LF)
 * 
 * @param line La línea de texto a verificar.
 * 
 * @return `true` si la línea está vacía o solo contiene caracteres en blanco, `false` en caso contrario.
 */
bool Reader::is_line_empty(std::string line)
{
  if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos)
    return true;
  return false;
}




std::string Reader::getExtension(const std::string& nombreArchivo) {
    // Buscar el último punto en el nombre del archivo
    size_t puntoPos = nombreArchivo.find_last_of('.');
    
    // Si no se encontró ningún punto, no hay extensión
    if (puntoPos == std::string::npos) {
        return "";
    }

    // Obtener la extensión a partir del punto
    return nombreArchivo.substr(puntoPos + 1);
}



void Reader::read_obj(const std::string& filename)
{
      // Intentar abrir el archivo dado.
      std::ifstream file(filename);
      if (!file.is_open()) {
          std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
      }

      // Iterar sobre cada linea del .obj
      std::string line;
      while (std::getline(file, line)) {
          // Dividir la linea en sus partes.
          std::istringstream iss(line);
          
          // Leer primera letra, esta puede indicar:
          // v: vertice
          // vn: normal del vertice
          // vt: pos del vertice en textura
          // f: poligono
          // l: linea 
          // o: objetos
          // s: grupo de normalizacion
          // #: comentario
          // Para la lectura solo se utilizara v y f.
          std::string token;
          iss >> token;

          if (token == "v") {
              // Leer y asignar vertice a su estructura de datos respectiva.
              float x, y, z;
              iss >> x >> y >> z;
              Vertex ver = { glm::vec3(x, y, z), glm::vec3(0.0f)};
              vertices.push_back(ver);
          } else if (token == "f") {
              // Leer y asignar caras, soporta tris y quads.

              std::vector <int> indexs;
              int numero;
              while (!iss.eof()) {
                  if (iss >> numero) {
                      // Asignar numero del indice que compone la cara
                      indexs.push_back((GLuint) numero);
                  } else {
                      // Si no quedan mas numero por extraer.
                      break;
                  }
              }
              
              // Crear un Tri, si es la cara estaba compuesta de 3 vertices.
              if (indexs.size() == 3)
              {
                  int i3 = indexs.back();
                  indexs.pop_back();

                  int i2 = indexs.back();
                  indexs.pop_back();

                  int i1 = indexs.back();
                  indexs.pop_back();

                  Tri tri1 = {i1 - 1, i2 - 1, i3 - 1};
                  tris.push_back(tri1);

              } else if (indexs.size() == 4)
              {
              // Crear dos Tris, si es que la cara esta compuesta de 4 vertices.
                  int i4 = indexs.back();
                  indexs.pop_back();

                  int i3 = indexs.back();
                  indexs.pop_back();

                  int i2 = indexs.back();
                  indexs.pop_back();

                  int i1 = indexs.back();
                  indexs.pop_back();

                  Tri tri1 = {i1 - 1, i2 - 1, i3 - 1};
                  Tri tri2 = {i1 - 1, i3 - 1, i4 - 1};

                  tris.push_back(tri1);
                  tris.push_back(tri2);

              } else {
                // En caso de que se ingresen caras con mas de 4 vertices.
                  std::cout << "Cantidad de caras no soportadas" << std::endl;
              }
              
          }
      }
      
      // Cerrar archivo
      file.close();

}



void Reader::read_vtk(const std::string& filename)
{
    // Intentar abrir el archivo
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    }


    std::string line;

    int line_count = 0;




    bool points_getted = false;
    bool cells_getted = false;
    bool cellstype_getted = false;
    std::string structure;
    bool debug = true;
    
    
    // VTK HEADERS:
    // - VTK Version
    // - VTK Name
    // - File Codification
    // - Dataset Structure
    
    std::string version = "";
    std::string name = "";
    std::string codification = "";
    std::string dataset  = "";
    

    // Leer cabecera del archivo .vtk
    // El formato general es:
    // # Version del archivo
    // Nombre del modelo
    // Codificacion de la informacion (ASCII | BINARY)
    // Tipos de datos (ImageData, RectilinearGrid, StructuredGrid, PolyData, UnstructuredGrid)
    while (std::getline(file, line)) {

        if (is_line_empty(line))
            continue;
        
        if(version.empty()) 
        {
          version = line;
          continue;
        }
        
        if (name.empty())
        {
          name = line;
          continue;
        }
        
        if (codification.empty())
        {
          codification = line;
          continue;
        }
        
        if (dataset.empty())
        {
          std::istringstream iss(line);
          std::string temp;
          iss >> temp >> dataset;
          break;
        }
    }
    
    if (dataset != "UNSTRUCTURED_GRID" && dataset != "unstructured_grid")
    {
      std::cout << "Tipo de Dataset no soportado" << std::endl;
      return ;
    }
   
   
    // Conseguir Encabezado de vertices POINTS NUM_PUNTOS TIPO_DE_DATO
    std::string points, tipo;
    int points_num;
    
    while (!points_getted) // Iterar hasta conseguir la cantidad de puntos
    {
        std::getline(file, line);
        if (is_line_empty(line))
            continue;
        std::istringstream iss(line);
        
        points_getted = true;

        // Asignar variable con el total de puntos a leer y el tipo de dato respectivo.
        iss >> points >> points_num >> tipo;

    }
    
    
    // Iterar hasta extrar la cantidad de points_num
    std::vector <glm::vec3> points_vector; // Variable que todos los puntos del espacio 3D leidos.
    std::vector <float> each_point; // Variable para asignar cada una de las componentes de los puntos leidos
    // Util cuando en los archivos las componentes vienen dadas en algun formato distinto a: x,y,z, por ejemplo 0.0 0.0 0.0 0.0 0.0
    
    bool points_extracted  = false;
    while (!points_extracted)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss(line);

        // Variable componente
        float n;
        // Almacenar cada componente.
        while (iss >> n)
            each_point.push_back(n);

        // Verificar si se extrajeron todos los puntos informados
        if ( int(each_point.size() / 3) >= points_num)
            points_extracted = true;
    }
    
    int init = 0;
    for (int i = 0; i < points_num; i += 1)
    {
       // TODO: Eliminar del vertice el valor del color.
        vertices.push_back({ glm::vec3(each_point[init], each_point[init+1], each_point[init+2]), glm::vec3(0.0f), glm::vec3(0.5f)});
        init +=3;
    }

    std::string cells;
    int n_cells, n_indexs;

    // Conseguir Encabezado CELLS NUM_CELLS SIZE_INDEXS
    while (!cells_getted)
    {
        std::getline(file, line);
        std::istringstream iss_cells(line);
        if (is_line_empty(line))
            continue;
        // Asignar Numero total de celdas y la cantidad todal de indices.
        iss_cells >> cells >> n_cells >> n_indexs;
        cells_getted = true;
    }
  
  
    // Extraer las celdas y las asociones con sus respectivas caras.
    for (int i = 0; i < n_cells; i++)
    {
        std::getline(file, line);
        std::istringstream iss_indexs(line);

        int indexs;
        iss_indexs >> indexs;

        std::vector <int> temp_index_vec;

        for (int j = 0; j < indexs; j++)
        {
            int i_;
            iss_indexs >> i_;
            temp_index_vec.push_back(i_);
        }
        indices.push_back(temp_index_vec);
    }

    // Conseguir Encabezado CELL_TYPES NUM_TYPE
    std::string cells_types;
    int n_cells_types;
    while (!cellstype_getted)
    {
        std::getline(file, line);
        std::istringstream iss_cells(line);

        if (is_line_empty(line))
            continue;

        // Guardar numero de celdas
        iss_cells >> cells_types >> n_cells_types;
        cellstype_getted = true;
    }
    
    if (n_cells_types > 0)
        has_polyhedrons = true;

    // Extraer numero del tipo de celdas
    for (int i = 0; i < n_cells; i++)
    {
        std::getline(file, line); 
        std::istringstream iss_type(line);
        int tipo;
        iss_type >> tipo;
        cells_types_vector.push_back(tipo);
    }


}
