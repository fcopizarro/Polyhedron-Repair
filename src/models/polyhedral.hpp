#pragma once

#include "common.hpp"
#include <memory>



class Polyhedral
{
protected:
    std::vector <float> lengths;
    std::vector<std::shared_ptr<Vertex>> vertexs_refs;
    
public:
    std::vector <float> J, JR, Jens;
    float AR, ARG, ARen, EQ;
    virtual void CalculateJ(){}
    virtual void CalculateJR(){}
    virtual void CalculateAR(){}
    virtual void CalculateARG(){}
    virtual void CalculateJENS(){}
    virtual void CalculateEQ(){}
    virtual void CalculateAREN(){}

    Polyhedral(std::vector<std::shared_ptr<Vertex>>);
    void GiveColor(glm::vec3, glm::vec3);
};

class Hexaedral: public virtual Polyhedral
{
private:
    float k = 1.0f;
public:
    Hexaedral(std::vector<std::shared_ptr<Vertex>>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;
    
};

class Tetrahedra: public virtual Polyhedral
{
private:
    float kens = sqrt(2) / 2;
public:
    Tetrahedra(std::vector<std::shared_ptr<Vertex>>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;
};

class Pyramid: public virtual Polyhedral
{
private:
    float kens_base = sqrt(6) / 3;
    float kens_apex = 2 * sqrt(6) / 9;
    float kar = sqrt(6) / 3;
    std::vector<glm::vec3> midpoints;
public:
    Pyramid(std::vector<std::shared_ptr<Vertex>>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;
};

class Prism: public virtual Polyhedral
{
private:
    float kens= sqrt(3) / 3;
    float kar = sqrt(6) / 3;
public:
    Prism(std::vector<std::shared_ptr<Vertex>>&);
    void CalculateJ() override;
    void CalculateJR() override;
    void CalculateAR() override;
    void CalculateARG() override;
    void CalculateJENS() override;
    void CalculateEQ() override;
    void CalculateAREN() override;
};


// Mesh compuesto por uno o mas polihedros
class Polyhedral_Mesh
{
private:
    std::vector <glm::vec3> shader_vertexs;
    std::vector <int> types;
    std::vector <std::vector <int>> indexs; 
    
public:
    std::vector <glm::vec3> vertexs;



    std::vector <std::shared_ptr<Polyhedral>> polys;
    Polyhedral_Mesh(/* args */);
    void PushVertex(glm::vec3);
    void PushType(int);
    void PushIndex(std::vector<int>);
    void toString();
    void FormPolys(std::vector<std::shared_ptr<Vertex>>) ;
    void CalculateJ();
    void GetJ();

    std::vector<std::shared_ptr<Vertex> > toVertex();
    std::vector<Tri> toTris();

    bool includeHexa = true, includeTetra = true, includePyra = true, includePrism = true;
    bool includeHexa2 = true, includeTetra2 = true, includePyra2 = true, includePrism2 = true;

    std::vector<float> Jtotal, Jdata, JRdata, JENSdata, EQdata;
    std::vector<float> ARtotal, ARdata, ARGdata, ARENdata;

    std::vector<float> histogramData;
    std::vector<float> histogramLabels;

    std::vector<float> JData;

    int qtyHexa = 0, qtyTetra = 0, qtyPyra = 0, qtyPrism = 0;
    
};

