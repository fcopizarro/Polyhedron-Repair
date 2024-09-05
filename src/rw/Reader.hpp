#pragma once    

#include "common.hpp"

class Reader
{
public:
    Reader();
    void read_file(const std::string);
    std::vector<Vertex> vertices;
    std::vector<Tri> tris;
    
    bool has_polyhedrons = false;
    std::vector <std::vector <int>> indices;
    std::vector <int> cells_types_vector;

    
    bool read_status = false; // Estado de final de lectura. True si se leyo el formato, False si hubo un problema.
    
private:
    
    bool is_line_empty(std::string );
    std::string getExtension(const std::string&);
    void read_obj(const std::string &);
    void read_vtk(const std::string &);
    


    
    
    // Auxiliares para definir el bounding box y centro del objeto cargado
    // Permitiendo a la camara fijar el punto central.
    //glm::vec3 min_ (std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    //glm::vec3 max_ (std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    

};
