#pragma once

#include "common.hpp"
#include <memory>



class Polyhedron
{
protected:
    std::vector <float> lengths;
    
public:
    std::vector<Vertex*> vertexs_refs;
    std::vector <float> J, JR, Jens;
    float AR, ARG, ARen, EQ;
    virtual void CalculateJ(){}
    virtual void CalculateJR(){}
    virtual void CalculateAR(){}
    virtual void CalculateARG(){}
    virtual void CalculateJENS(){}
    virtual void CalculateEQ(){}
    virtual void CalculateAREN(){}
    virtual std::tuple<int, int, int> GetAdjs(int) = 0;

    virtual float CalculateJ_index(int ) = 0;
    virtual float SimulateMoveJ(int , const glm::vec3&) = 0;

    virtual float CalculateJR_index(int ) = 0;
    virtual float SimulateMoveJR(int , const glm::vec3&) = 0;

    virtual float CalculateJens_index(int ) = 0;
    virtual float SimulateMoveJens(int , const glm::vec3&) = 0;

    Polyhedron(const std::vector<Vertex*>&);
    void GiveColor(glm::vec3, glm::vec3);
    bool FixJ(float , int );
    glm::vec3 GenerateRandomMove();
    float SimulateJ(glm::vec3 );
    bool EasyFix(float t, int, int);
    bool GradFix(float t, int maxtrys, int selected_metric);
    

};

class Hexaedron: public virtual Polyhedron
{
private:
    float k = 1.0f;
public:
    Hexaedron(const std::vector<Vertex*>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;

    float CalculateJ_index(int index) override;
    float SimulateMoveJ(int , const glm::vec3& ) override;

    float CalculateJR_index(int index) override;
    float SimulateMoveJR(int , const glm::vec3& ) override;

    float CalculateJens_index(int index) override;
    float SimulateMoveJens(int , const glm::vec3& ) override;

    std::tuple<int, int, int> GetAdjs(int) override;
    
};

class Tetrahedron: public virtual Polyhedron
{
private:
    float kens = sqrt(2) / 2;
public:
    Tetrahedron(const std::vector<Vertex*>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;

    float CalculateJ_index(int index) override;
    float SimulateMoveJ(int , const glm::vec3&) override;

    float CalculateJR_index(int index) override;
    float SimulateMoveJR(int , const glm::vec3& ) override;

    float CalculateJens_index(int index) override;
    float SimulateMoveJens(int , const glm::vec3& ) override;


    std::tuple<int, int, int> GetAdjs(int) override;
};

class Pyramid: public virtual Polyhedron
{
private:
    float kens_base = sqrt(6) / 3;
    float kens_apex = 2 * sqrt(6) / 9;
    float kar = sqrt(6) / 3;
    std::vector<glm::vec3> midpoints;
public:
    Pyramid(const std::vector<Vertex*>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;

    float CalculateJ_index(int index) override;
    float SimulateMoveJ(int , const glm::vec3& ) override;

    float CalculateJR_index(int index) override;
    float SimulateMoveJR(int , const glm::vec3& ) override;

    float CalculateJens_index(int index) override;
    float SimulateMoveJens(int , const glm::vec3& ) override;


    std::tuple<int, int, int> GetAdjs(int) override;
};

class Prism: public virtual Polyhedron
{
private:
    float kens= sqrt(3) / 3;
    float kar = sqrt(6) / 3;
public:
    Prism(const std::vector<Vertex*>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;

    float CalculateJ_index(int index) override;
    float SimulateMoveJ(int , const glm::vec3& ) override;

    float CalculateJR_index(int index) override;
    float SimulateMoveJR(int , const glm::vec3& ) override;

    float CalculateJens_index(int index) override;
    float SimulateMoveJens(int , const glm::vec3& ) override;


    std::tuple<int, int, int> GetAdjs(int) override;
};


// Clase para la malla de poliedros
class Polyhedral_Mesh
    {
    private:
        std::vector <glm::vec3> vertices;
        
        // Mapa para asociar cada índice de vértice con una lista de pares (poliedro, índice del vértice en el poliedro)
        std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> vertex_to_polyhedron_map;
        
        
    public:
    
        
        std::string MapVertexIndex(int);
        std::vector <int> types;
        std::vector <std::vector <int>> indexs; 
        std::vector <std::shared_ptr<Polyhedron>> polyhedrons;
        Polyhedral_Mesh();
        void BindPolyhedronsInfo(std::vector <std::vector<int>>, std::vector <int> );
        
        void CreatePolyhedrons(const std::vector<Vertex>&);
        void toString();

        void CalculateJ();

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

