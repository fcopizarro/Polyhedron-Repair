#pragma once

#include "common.hpp"


/**
 * @class Polyhedron
 * @brief Clase abstracta de poliedro, proporciona la estructura necesaria para cada uno de los poliedros especificos.
 * 
 */
class Polyhedron
{
    // Clase abstracta que será utilizada como estructura de cada uno de los poliedros
protected:
    // Atributos 
    std::vector <float> lengths; // Largos de las aristas
    

public:

    std::vector<Vertex*> vertices_ptr; // Puntero hacia los vectores globales
    std::vector <float> Js, JR, Jens; // Basadas en el Jacobiano
    float AR, ARG, ARen; // Basadas en la relacion de aspecto
    Polyhedron(const std::vector<Vertex*>&); // Constructor

    // Metodos para calcular las distintas metricas por separado
    virtual void CalculateJs(){}
    // virtual void CalculateJR(){}
    virtual void CalculateAR(){}
    virtual void CalculateARG(){}
    virtual void CalculateJENS(){}
    virtual void CalculateAREN(){}

    // Metodo para conseguir el indice de los vertices adjuntos a un vertice.
    virtual std::tuple<int, int, int> GetAdjs(int) = 0;

    // Metodos para calcular las metricas en un vertice individual
    // Util para simular las metricas en los movimientos
    virtual float SimulateMoveJs(int , const glm::vec3&) = 0;
    // virtual float CalculateJR_index(int ) = 0;
    // virtual float SimulateMoveJR(int , const glm::vec3&) = 0;
    virtual float SimulateMoveJens(int , const glm::vec3&) = 0;
    
    // Metodos para reparar los poliedros
    glm::vec3 GenerateRandomMove(); // Funcion para generar un paso aleatorio en un espacio 3D
    bool EasyFix(float t, int, int); // Reparacion en base a pasos aleatorios
    bool GradFix(float t, int maxtrys, int selected_metric); // Reparacion en base a busca de un gradiante
    

};


/**
 * @class Hexaedron
 * @brief Representa un hexaedro, una figura poliedral con 8 vértices, 12 aristas y 6 caras.
 * 
 * La clase `Hexaedron` hereda de `Polyhedron` e implementa métodos específicos para el cálculo de métricas
 * y simulación de movimientos asociados a un hexaedro.
 */
class Hexaedron: public virtual Polyhedron
{
private:
    float k = 1.0f; // Constante usada para el calculo de algunas metricas.
public:

    /**
     * @brief Constructor de la clase Hexaedron.
     * 
     * Inicializa un objeto de la clase `Hexaedron` utilizando un vector de punteros a vértices.
     * 
     * @param vertices_ptr Un vector de punteros a objetos de la clase `Vertex`, que representan los vértices del hexaedro.
     */
    Hexaedron(const std::vector<Vertex*>&);

    // Metodo para conseguir el indice de los vertices adjuntos a un vertice.
    std::tuple<int, int, int> GetAdjs(int) override;

    // Metodos para Calcular metricas
    void CalculateJs() override;
    //void CalculateJR() override;
    void CalculateAR() override;
    void CalculateJENS() override;
    //void CalculateEQ() override;
    void CalculateAREN() override;

    // Metodos para reparar los poliedros
    float SimulateMoveJs(int , const glm::vec3& ) override;
    //float CalculateJR_index(int index) override;
    //float SimulateMoveJR(int , const glm::vec3& ) override;
    float SimulateMoveJens(int , const glm::vec3& ) override;

    
};


/**
 * @class Tetrahedron
 * @brief Representa un tetraedro, una figura poliedral con 4 vértices, 6 aristas y 4 caras.
 * 
 * La clase `Tetrahedron` hereda de `Polyhedron` e implementa métodos específicos para el cálculo de métricas
 * y simulación de movimientos asociados a un tetraedro.
 */
class Tetrahedron: public virtual Polyhedron
{
private:
    float kens = sqrt(2) / 2;
public:
    Tetrahedron(const std::vector<Vertex*>&);
    void CalculateJs() override;
    //void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    //void CalculateEQ() override;
    void CalculateAREN() override;

    float SimulateMoveJs(int , const glm::vec3&) override;

    //float CalculateJR_index(int index) override;
    //float SimulateMoveJR(int , const glm::vec3& ) override;

    float SimulateMoveJens(int , const glm::vec3& ) override;


    std::tuple<int, int, int> GetAdjs(int) override;
};


/**
 * @class Pyramid
 * @brief Representa una piramide, una figura poliedral con 5 vértices, 8 aristas y 5 caras.
 * 
 * La clase `Pyramid` hereda de `Polyhedron` e implementa métodos específicos para el cálculo de métricas
 * y simulación de movimientos asociados a una píramide.
 */
class Pyramid: public virtual Polyhedron
{
private:
    float kens_base = sqrt(6) / 3;
    float kens_apex = 2 * sqrt(6) / 9;
    float kar = sqrt(6) / 3;
    std::vector<glm::vec3> midpoints;
public:
    Pyramid(const std::vector<Vertex*>&);
    void CalculateJs() override;
    //void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    //void CalculateEQ() override;
    void CalculateAREN() override;

    float SimulateMoveJs(int , const glm::vec3& ) override;

    //float CalculateJR_index(int index) override;
    //float SimulateMoveJR(int , const glm::vec3& ) override;

    float SimulateMoveJens(int , const glm::vec3& ) override;


    std::tuple<int, int, int> GetAdjs(int) override;
};


/**
 * @class Prism
 * @brief Representa un prisma, una figura poliedral con 6 vértices, 9 aristas y 5 caras.
 * 
 * La clase `Prism` hereda de `Polyhedron` e implementa métodos específicos para el cálculo de métricas
 * y simulación de movimientos asociados a un prisma.
 */
class Prism: public virtual Polyhedron
{
private:
    float kens= sqrt(3) / 3;
    float kar = sqrt(6) / 3;
public:
    Prism(const std::vector<Vertex*>&);
    void CalculateJs() override;
    //void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    //void CalculateEQ() override;
    void CalculateAREN() override;

    float SimulateMoveJs(int , const glm::vec3& ) override;

    //float CalculateJR_index(int index) override;
    //float SimulateMoveJR(int , const glm::vec3& ) override;

    float SimulateMoveJens(int , const glm::vec3& ) override;


    std::tuple<int, int, int> GetAdjs(int) override;
};


/**
 * @class Polyhedral_Mesh
 * @brief Malla de poliedros, es la principal union entre todos los poliedros instanciados, permitiendo la interaccion directa con ellos.
 * 
 * Version poliedrica de la malla ingresada.
 */
class Polyhedral_Mesh
    {
    private:
        std::vector <glm::vec3> vertices;
        
        // Mapa para asociar cada índice de vértice con una lista de pares (poliedro, índice del vértice en el poliedro)
        std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> vertex_to_polyhedron_map;
        
        
    public:
    
        
        std::string MapVertexIndex(int);
        std::vector <int> types;
        std::vector <std::vector <int>> indices; 
        std::vector <std::shared_ptr<Polyhedron>> polyhedrons;
        Polyhedral_Mesh();
        void BindPolyhedronsInfo(std::vector <std::vector<int>>, std::vector <int> );
        
        void CreatePolyhedrons(const std::vector<Vertex>&);
        void toString();

        void CalculateMetrics();

        std::vector<Tri> toTris();

        bool includeHexa = true, includeTetra = true, includePyra = true, includePrism = true;
        bool includeHexa2 = true, includeTetra2 = true, includePyra2 = true, includePrism2 = true;

        std::vector<float> Jtotal, Jdata, JRdata, JENSdata, EQdata;
        std::vector<float> ARtotal, ARdata, ARGdata, ARENdata;

        std::vector<float> histogramData;
        std::vector<float> histogramLabels;

        std::vector<float> JData;

        int qtyHexa = 0, qtyTetra = 0, qtyPyra = 0, qtyPrism = 0;

        void FixJ(float , int, int, int);
        
    };

