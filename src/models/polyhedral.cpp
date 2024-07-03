#include "polyhedral.hpp"

Polyhedral::Polyhedral(std::vector<std::shared_ptr<Vertex>>& a)
{
    
}

void Polyhedral::GiveColor(glm::vec3 color1, glm::vec3 color2)
{   
    // c1 -> -1
    // c2 -> 1

    std::cout << " AAAAAAAAAAAAAA\n" ;

    for (auto ver: vertexs_refs)
    {
        //(*ver).color = glm::vec3(1.0f, 0.0f, 0.0f);
        std::cout << (*ver).color.x << " "  << (*ver).color.y << " " << (*ver).color.z << std::endl;
    }


}


Hexaedral::Hexaedral(std::vector<std::shared_ptr<Vertex>>& vasad) : Polyhedral(vasad)
{
    vertexs_refs = vasad;
}
void Hexaedral::CalculateJ() 
{
    //std::cout << "Calculando J" << std::endl;
    
    // Js
    // 0 -> 1 3 4
    // 1 -> 0 2 5
    // 2 -> 1 3 6
    // 3 -> 0 2 7
    // 4 -> 0 5 7
    // 5 -> 1 4 6
    // 6 -> 2 5 7
    // 7 -> 3 4 6
    glm::vec3 origin;
    glm::vec3 normalizedv1, normalizedv2, normalizedv3 ;
    
    origin = (*vertexs_refs[0]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[4]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[1]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[5]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[2]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[2]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[6]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[3]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[3]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[7]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));


    origin = (*vertexs_refs[4]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[7]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[5]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[5]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[4]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[6]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[6]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[5]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[7]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[7]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[6]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[4]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));



}
void Hexaedral::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }

}
void Hexaedral::CalculateAR()
{
    float min_dist = std::numeric_limits<float>::max();
    float max_dist = std::numeric_limits<float>::lowest();

    float dist = glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[1]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[2]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;
        
    dist = glm::distance((*vertexs_refs[2]).position, (*vertexs_refs[3]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[3]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[4]).position, (*vertexs_refs[5]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[5]).position, (*vertexs_refs[6]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[6]).position, (*vertexs_refs[7]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[7]).position, (*vertexs_refs[4]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[4]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[5]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[2]).position, (*vertexs_refs[6]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertexs_refs[3]).position, (*vertexs_refs[7]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    AR = min_dist / max_dist;

} 
void Hexaedral::CalculateJENS()
{
    // Jens = JS, con k = 1.
    Jens = J;
}
void Hexaedral::CalculateEQ()
{
    float min = 1.0f;
    float max_negative = -1.0f;
    bool exist_negative = false;

    for(float Jens_: Jens)
    {
        if (Jens_ < 0)
        {
            exist_negative = true;
            if (max_negative < Jens_)
                max_negative = Jens_;
        }

        if (min > Jens_)
            min = Jens_;
        
    }


    if (exist_negative)
        EQ = max_negative;
    else
        EQ = min;

}
void Hexaedral::CalculateAREN()
{
    ARen = AR;
}

Tetrahedra::Tetrahedra(std::vector<std::shared_ptr<Vertex>>& vasad) : Polyhedral(vasad)
{
    vertexs_refs = vasad;
}
void Tetrahedra::CalculateJ() 
{
    //std::cout << "Calculando J Tetra" << std::endl;
    
    // Js
    // 0 -> 1 2 3
    // 1 -> 0 2 3
    // 2 -> 0 1 3
    // 3 -> 0 1 2
    glm::vec3 origin;
    glm::vec3 normalizedv1, normalizedv2, normalizedv3 ;
    
    origin = (*vertexs_refs[0]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[3]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[1]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[2]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[2]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[3]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[3]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[1]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

}
void Tetrahedra::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }

}
void Tetrahedra::CalculateAR()
{
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[1]).position));
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[2]).position));
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[3]).position));
    lengths.push_back(glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[2]).position));
    lengths.push_back(glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[3]).position));
    lengths.push_back(glm::distance((*vertexs_refs[2]).position, (*vertexs_refs[3]).position));

    float min_dist = std::numeric_limits<float>::max();
    float max_dist = std::numeric_limits<float>::lowest();

    for(float length: lengths){

        if (length < min_dist)
            min_dist = length;
        
        if (length > max_dist)
            max_dist = length;
    }

    AR = min_dist / max_dist;
}
void Tetrahedra::CalculateARG()
{
    float R = 0.0f;

    for (float  length: lengths)
    {
        R += pow(length, 2);
    }

    R /= 6.0f;

    R = sqrt(R);

    // A = (*vertexs_refs[0]).position;
    // B = (*vertexs_refs[1]).position;
    // C = (*vertexs_refs[2]).position;
    // D = (*vertexs_refs[3]).position;

    // abs (glm::dot( B - C, glm::cross( C - A, D - A)) / 6.0f

    float V = abs(glm::dot( (*vertexs_refs[1]).position - (*vertexs_refs[2]).position, glm::cross( (*vertexs_refs[2]).position - (*vertexs_refs[0]).position, (*vertexs_refs[3]).position - (*vertexs_refs[0]).position))) / 6.0f;
    
    ARG = (pow(R, 3) * sqrt(2)) / (12 * V);
} 
void Tetrahedra::CalculateJENS()
{

    for (float J_: J)
    {
        if (J_ > kens)
        {
            Jens.push_back( (1 + kens) - J_ );
        }
        else if ( -kens <= J_ && J_ <= kens )
        {
            Jens.push_back( J_ / kens );
        }
        else if (J_ < -kens)
        {
            Jens.push_back( -1 * (1 + kens) - J_ );
        }
    }
}
void Tetrahedra::CalculateEQ()
{
    float min = 1.0f;
    float max_negative = -1.0f;
    bool exist_negative = false;

    for(float Jens_: Jens)
    {
        if (Jens_ < 0)
        {
            exist_negative = true;
            if (max_negative < Jens_)
                max_negative = Jens_;
        }

        if (min > Jens_)
            min = Jens_;
        
    }


    if (exist_negative)
        EQ = max_negative;
    else
        EQ = min;

}
void Tetrahedra::CalculateAREN()
{
    ARen = AR;
}

Pyramid::Pyramid(std::vector<std::shared_ptr<Vertex>>& vasad) : Polyhedral(vasad)
{
    vertexs_refs = vasad;
}
void Pyramid::CalculateJ() 
{
    // Se trata como un hexaedro mal formado, calculando los 4 puntos gaussianos en la puntos de la piramide.
    midpoints.push_back(glm::mix((*vertexs_refs[0]).position, (*vertexs_refs[4]).position, 0.5f));
    midpoints.push_back(glm::mix((*vertexs_refs[1]).position, (*vertexs_refs[4]).position, 0.5f));
    midpoints.push_back(glm::mix((*vertexs_refs[2]).position, (*vertexs_refs[4]).position, 0.5f));
    midpoints.push_back(glm::mix((*vertexs_refs[3]).position, (*vertexs_refs[4]).position, 0.5f));

    glm::vec3 origin;
    glm::vec3 normalizedv1, normalizedv2, normalizedv3 ;
    
    origin = (*vertexs_refs[0]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv3 = glm::normalize( midpoints[0] - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[1]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( midpoints[1] - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[2]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[2]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( midpoints[2] - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[3]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[3]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( midpoints[3] - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    std::vector <float> values_gauss; 
    origin = midpoints[0];
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( midpoints[3] - origin );
    normalizedv3 = glm::normalize( midpoints[1] - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = midpoints[1];
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( midpoints[0] - origin );
    normalizedv3 = glm::normalize( midpoints[2] - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = midpoints[2];
    normalizedv1 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv2 = glm::normalize( midpoints[1] - origin );
    normalizedv3 = glm::normalize( midpoints[3] - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = midpoints[3];
    normalizedv1 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv2 = glm::normalize( midpoints[2] - origin );
    normalizedv3 = glm::normalize( midpoints[0] - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    float min_ele = *min_element(values_gauss.begin(), values_gauss.end());
    J.push_back(min_ele);
}
void Pyramid::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }

}
void Pyramid::CalculateAR()
{
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[1]).position));
    lengths.push_back(glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[2]).position));
    lengths.push_back(glm::distance((*vertexs_refs[2]).position, (*vertexs_refs[3]).position));
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[3]).position));
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[4]).position));
    lengths.push_back(glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[4]).position));
    lengths.push_back(glm::distance((*vertexs_refs[2]).position, (*vertexs_refs[4]).position));
    lengths.push_back(glm::distance((*vertexs_refs[3]).position, (*vertexs_refs[4]).position));

    float min_dist = std::numeric_limits<float>::max();
    float max_dist = std::numeric_limits<float>::lowest();

    for(float length: lengths){

        if (length < min_dist)
            min_dist = length;
        
        if (length > max_dist)
            max_dist = length;
    }

    AR = min_dist / max_dist;
}
void Pyramid::CalculateARG()
{
   
} 
void Pyramid::CalculateJENS()
{

    float kens_ = 0.0f;

    for (int i = 0; i < J.size(); i++)
    {
        if (i == 4)
            kens_ = kens_apex;
        else 
            kens_ = kens_base;

        float J_ = J[i];

        if (J_ > kens_)
        {
            Jens.push_back( (1 + kens_) - J_ );
        }
        else if ( -kens_ <= J_ && J_ <= kens_ )
        {
            Jens.push_back( J_ / kens_ );
        }
        else if (J_ < -kens_)
        {
            Jens.push_back( -1 * (1 + kens_) - J_ );
        }
    }
    
}
void Pyramid::CalculateEQ()
{
    float min = 1.0f;
    float max_negative = -1.0f;
    bool exist_negative = false;

    for(float Jens_: Jens)
    {
        if (Jens_ < 0)
        {
            exist_negative = true;
            if (max_negative < Jens_)
                max_negative = Jens_;
        }

        if (min > Jens_)
            min = Jens_;
        
    }


    if (exist_negative)
        EQ = max_negative;
    else
        EQ = min;
}
void Pyramid::CalculateAREN()
{
    if (AR > kar)
        ARen = (1+kar) - AR;
    else
        ARen = AR/kar;
}


Prism::Prism(std::vector<std::shared_ptr<Vertex>>& vasad) : Polyhedral(vasad)
{
    vertexs_refs = vasad;
}
void Prism::CalculateJ() 
{
    //std::cout << "Calculando J Tetra" << std::endl;
    
    // Js
    // 0 -> 1 2 3
    // 1 -> 0 2 3
    // 2 -> 0 1 3
    // 3 -> 0 1 2
    glm::vec3 origin;
    glm::vec3 normalizedv1, normalizedv2, normalizedv3 ;
    
    origin = (*vertexs_refs[0]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[3]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[1]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[4]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[2]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[2]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[5]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[3]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[5]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[4]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[4]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[5]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[5]).position;
    normalizedv1 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[4]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[3]).position - origin );
    J.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
}
void Prism::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }
}
void Prism::CalculateAR()
{
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[1]).position));
    lengths.push_back(glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[2]).position));
    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[2]).position));

    lengths.push_back(glm::distance((*vertexs_refs[3]).position, (*vertexs_refs[4]).position));
    lengths.push_back(glm::distance((*vertexs_refs[4]).position, (*vertexs_refs[5]).position));
    lengths.push_back(glm::distance((*vertexs_refs[3]).position, (*vertexs_refs[5]).position));

    lengths.push_back(glm::distance((*vertexs_refs[0]).position, (*vertexs_refs[3]).position));
    lengths.push_back(glm::distance((*vertexs_refs[1]).position, (*vertexs_refs[4]).position));
    lengths.push_back(glm::distance((*vertexs_refs[2]).position, (*vertexs_refs[5]).position));

    float min_dist = std::numeric_limits<float>::max();
    float max_dist = std::numeric_limits<float>::lowest();

    for(float length: lengths){

        if (length < min_dist)
            min_dist = length;
        
        if (length > max_dist)
            max_dist = length;
    }

    AR = min_dist / max_dist;
}
void Prism::CalculateARG()
{
   
} 
void Prism::CalculateJENS()
{

    
    for (int i = 0; i < J.size(); i++)
    {
        float J_ = J[i];

        if (J_ > kens)
        {
            Jens.push_back( (1 + kens) - J_ );
        }
        else if ( -kens <= J_ && J_ <= kens )
        {
            Jens.push_back( J_ / kens );
        }
        else if (J_ < -kens)
        {
            Jens.push_back( -1 * (1 + kens) - J_ );
        }
    }
}
void Prism::CalculateEQ()
{
    float min = 1.0f;
    float max_negative = -1.0f;
    bool exist_negative = false;

    for(float Jens_: Jens)
    {
        if (Jens_ < 0)
        {
            exist_negative = true;
            if (max_negative < Jens_)
                max_negative = Jens_;
        }

        if (min > Jens_)
            min = Jens_;
        
    }


    if (exist_negative)
        EQ = max_negative;
    else
        EQ = min;
}
void Prism::CalculateAREN()
{
    if (AR > kar)
        ARen = (1+kar) - AR;
    else
        ARen = AR/kar;
}






Polyhedral_Mesh::Polyhedral_Mesh()
{

}

void Polyhedral_Mesh::CalculateJ()
{
    for (auto poly: polys)
    {
        poly->CalculateJ();
        poly->CalculateJR();
        poly->CalculateAR();
        if (std::dynamic_pointer_cast<Tetrahedra>(poly) != nullptr)
            poly->CalculateARG();

        poly->CalculateJENS();
        poly->CalculateEQ();
        poly->CalculateAREN();
    }
}

void Polyhedral_Mesh::GetJ()
{
    Jtotal.clear();
    Jdata.clear();
    JRdata.clear();
    JENSdata.clear();
    EQdata.clear();


    ARtotal.clear();
    ARdata.clear();
    ARGdata.clear();
    ARENdata.clear();



    for (auto poly: polys)
    {
        if (std::dynamic_pointer_cast<Hexaedral>(poly) != nullptr)
        {
            if (includeHexa )
            {
                for (float J_: poly->J)
                    Jdata.push_back(J_);
                for (float J_: poly->JR)
                    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                EQdata.push_back(poly->EQ);
            }
            if(includeHexa2)
            {
                ARdata.push_back(poly->AR);
                ARGdata.push_back(poly->ARG);
                ARENdata.push_back(poly->ARen);
            }
            

        } else if (  std::dynamic_pointer_cast<Tetrahedra>(poly) != nullptr)
        {
            if (includeTetra )
            {
                for (float J_: poly->J)
                    Jdata.push_back(J_);
                for (float J_: poly->JR)
                    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                EQdata.push_back(poly->EQ);
            }
            if(includeTetra2)
            {
                ARdata.push_back(poly->AR);
                ARGdata.push_back(poly->ARG);
                ARENdata.push_back(poly->ARen);
            }

        } else if ( std::dynamic_pointer_cast<Pyramid>(poly) != nullptr)
        {
            if (includePyra )
            {
                for (float J_: poly->J)
                    Jdata.push_back(J_);
                for (float J_: poly->JR)
                    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                EQdata.push_back(poly->EQ);
            }
            if(includePyra2)
            {
                ARdata.push_back(poly->AR);
                ARGdata.push_back(poly->ARG);
                ARENdata.push_back(poly->ARen);
            }

        } else if ( std::dynamic_pointer_cast<Prism>(poly) != nullptr)
        {
            if (includePrism )
            {
                for (float J_: poly->J)
                    Jdata.push_back(J_);
                for (float J_: poly->JR)
                    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                EQdata.push_back(poly->EQ);
            }
            if(includePrism2)
            {
                ARdata.push_back(poly->AR);
                ARGdata.push_back(poly->ARG);
                ARENdata.push_back(poly->ARen);
            }

        }
        
    }


    for(float J_: Jdata)
        Jtotal.push_back(J_);
    for(float J_: JRdata)
        Jtotal.push_back(J_);
    for(float J_: JENSdata)
        Jtotal.push_back(J_);
    for(float J_: EQdata)
        Jtotal.push_back(J_);


    for(float AR_: ARdata)
        ARtotal.push_back(AR_);
    for(float AR_: ARGdata)
        ARtotal.push_back(AR_);
    for(float AR_: ARENdata)
        ARtotal.push_back(AR_);


}


void Polyhedral_Mesh::FormPolys()
{
    std::cout << "FormPolys call\nTamano types: " << types.size() << std::endl;
    for (int i = 0; i < types.size(); i++)
    {
        std::vector<std::shared_ptr<Vertex>> vertex_refs;

        for (int j = 0; j < indexs[i].size(); j++)
        {
            vertex_refs.push_back(std::make_shared<Vertex>(vertexs[indexs[i][j]]));
        }

        if (types[i] == 12)
        {
            polys.push_back( std::make_shared<Hexaedral>(vertex_refs) );
            qtyHexa ++;
        }
        else if (types[i] == 10)
        {
            polys.push_back( std::make_shared<Tetrahedra>(vertex_refs) );
            qtyTetra ++;
        }
        else if (types[i] == 14)
        {
            polys.push_back( std::make_shared<Pyramid>(vertex_refs) );
            qtyPyra ++;
        }
        else if (types[i] == 13)
        {
            polys.push_back( std::make_shared<Prism>(vertex_refs) );
            qtyPrism ++;
        }
    }
    
}


void Polyhedral_Mesh::PushVertex(glm::vec3 vertex)
{
    vertexs.push_back(vertex);
}

void Polyhedral_Mesh::PushType(int type_)
{
    types.push_back(type_);
}
void Polyhedral_Mesh::PushIndex(std::vector<int> index)
{
    indexs.push_back(index);
}
void Polyhedral_Mesh::toString()
{
    std::cout << "\n\nResumen Polyhedros\n N vertices: " << vertexs.size() << std::endl; 

    std::cout << "Imprimiendo vertices" << std::endl;

    for (int i = 0; i < vertexs.size(); i++)
    {
        std::cout << vertexs[i].x << " " << vertexs[i].y << " " << vertexs[i].z << std::endl;
    }

    std::cout << "\n\nImprimiendo valores de J" << std::endl;
    for(auto poly: polys)
    {
        for (float J_: (*poly).J)
        {
            std::cout << J_ << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    std::cout << "Imprimiendo valores de JR" << std::endl;
    for(auto poly: polys)
    {
        for (float J_: (*poly).JR)
        {
            std::cout << J_ << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    std::cout << "Imprimiendo valores de AR" << std::endl;
    for(auto poly: polys)
    {
        std::cout << (*poly).AR << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;


    std::cout << "Imprimiendo valores de ARG" << std::endl;
    for(auto poly: polys)
    {
        if (std::dynamic_pointer_cast<Tetrahedra>(poly) != nullptr)
            std::cout << (*poly).ARG << " ";

        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Imprimiendo valores de Jens" << std::endl;
    for(auto poly: polys)
    {
        for (float Jens_: (*poly).Jens)
        {
            std::cout << Jens_ << " ";
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Imprimiendo valor de EQ" << std::endl;
    for(auto poly: polys)
    {
        std::cout << (*poly).EQ << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Imprimiendo valores de ARen" << std::endl;
    for(auto poly: polys)
    {
        std::cout << (*poly).ARen << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "\n\n\n" << std::endl;
    
}

std::vector<Vertex> Polyhedral_Mesh::toVertex()
{
    std::vector <Vertex> converted;


    for (glm::vec3 ver_: vertexs)
    {
        Vertex ver = { ver_, glm::vec3(0.0f), glm::vec3(0.5f)};
        converted.push_back(ver);
    }
    

    return converted;


    


}
    
std::vector<Tri> Polyhedral_Mesh::toTris()  
{
    std::vector <Tri> converted;

    for (int i = 0; i < indexs.size(); i++)
    {

        switch (types[i])
        {
        case 12:

        // Js
    // 0 -> 1 3 4
    // 1 -> 0 2 5
    // 2 -> 1 3 6
    // 3 -> 0 2 7
    // 4 -> 0 5 7
    // 5 -> 1 4 6
    // 6 -> 2 5 7
    // 7 -> 3 4 6
        
            converted.push_back({indexs[i][0], indexs[i][2], indexs[i][1]});
            converted.push_back({indexs[i][0], indexs[i][3], indexs[i][2]});
            
            converted.push_back({indexs[i][0], indexs[i][1], indexs[i][4]});
            converted.push_back({indexs[i][1], indexs[i][5], indexs[i][4]});

            converted.push_back({indexs[i][0], indexs[i][4], indexs[i][3]});
            converted.push_back({indexs[i][3], indexs[i][4], indexs[i][7]});
            
            converted.push_back({indexs[i][4], indexs[i][5], indexs[i][6]});
            converted.push_back({indexs[i][4], indexs[i][6], indexs[i][7]});

            converted.push_back({indexs[i][1], indexs[i][2], indexs[i][5]});
            converted.push_back({indexs[i][2], indexs[i][6], indexs[i][5]});

            converted.push_back({indexs[i][2], indexs[i][3], indexs[i][7]});
            converted.push_back({indexs[i][2], indexs[i][7], indexs[i][6]});

            break;

        case 10:
            converted.push_back({indexs[i][0], indexs[i][2], indexs[i][1]});
            converted.push_back({indexs[i][0], indexs[i][1], indexs[i][3]});
            converted.push_back({indexs[i][1], indexs[i][2], indexs[i][3]});
            converted.push_back({indexs[i][0], indexs[i][3], indexs[i][2]});

            break;

        case 14:
            converted.push_back({indexs[i][0], indexs[i][2], indexs[i][1]});
            converted.push_back({indexs[i][0], indexs[i][3], indexs[i][2]});
            converted.push_back({indexs[i][0], indexs[i][1], indexs[i][4]});
            converted.push_back({indexs[i][1], indexs[i][2], indexs[i][4]});
            converted.push_back({indexs[i][2], indexs[i][3], indexs[i][4]});
            converted.push_back({indexs[i][0], indexs[i][4], indexs[i][3]});

            break;

        case 13:
            converted.push_back({indexs[i][0], indexs[i][2], indexs[i][1]});

            converted.push_back({indexs[i][0], indexs[i][1], indexs[i][3]});
            converted.push_back({indexs[i][1], indexs[i][4], indexs[i][3]});

            converted.push_back({indexs[i][3], indexs[i][4], indexs[i][5]});

            converted.push_back({indexs[i][0], indexs[i][3], indexs[i][2]});
            converted.push_back({indexs[i][2], indexs[i][3], indexs[i][5]});

            converted.push_back({indexs[i][1], indexs[i][5], indexs[i][4]});
            converted.push_back({indexs[i][1], indexs[i][2], indexs[i][5]});

            break;

        
        default:
            std::cout << "Tipo de celda no soportado.\n" << types[i] << std::endl;
            break;
        }
    }
    


    return converted;

}
