#include "polyhedral.hpp"
// Metricas
// {
// vectores y floats de las metricas
// nombre de la metrica
//
//



Polyhedron::Polyhedron(const std::vector<Vertex*>& a)
{}


Hexaedron::Hexaedron(const std::vector<Vertex*>& vasad) : Polyhedron(vasad)
{
    vertexs_refs = vasad;
}


/**
 * @brief Obtiene los índices de los vértices adyacentes a un vértice específico en un hexaedro.
 * 
 * Dado un índice de vértice dentro de un hexaedro, esta función devuelve una tupla con los índices de los tres vértices adyacentes.
 * 
 * @param index El índice del vértice para el cual se desean obtener los vértices adyacentes. Debe estar en el rango [0, 7] para un hexaedro.
 * 
 * @return Una `std::tuple<int, int, int>` que contiene los índices de los tres vértices adyacentes al vértice dado.
 *         Si el índice proporcionado no está en el rango válido, la función devuelve `{0, 0, 0}` como valor predeterminado.
 */
std::tuple<int, int, int> Hexaedron::GetAdjs(int index)
{
    switch (index)
    {
      case 0:
          return {1, 3, 4};
      case 1:
          return {0, 5, 2};
      case 2:
          return {1, 6, 3};
      case 3:
          return {0, 2, 7};
      case 4:
          return {0, 7, 5};
      case 5:
          return {1, 4, 6};
      case 6:
          return {2, 5, 7};
      case 7:
          return {3, 6, 4};
      default:
          return {0, 0, 0};
    }
}



/**
 * @brief Obtiene los índices de los vértices adyacentes a un vértice específico en un tetraedro.
 * 
 * Dado un índice de vértice dentro de un tetraedro, esta función devuelve una tupla con los índices de los tres vértices adyacentes.
 * 
 * @param index El índice del vértice para el cual se desean obtener los vértices adyacentes. Debe estar en el rango [0, 3] para un tetraedro.
 * 
 * @return Una `std::tuple<int, int, int>` que contiene los índices de los tres vértices adyacentes al vértice dado.
 *         Si el índice proporcionado no está en el rango válido, la función devuelve `{0, 0, 0}` como valor predeterminado.
 */
std::tuple<int, int, int> Tetrahedron::GetAdjs (int index)
{
    switch (index)
    {
    case 0:
        return {1, 2, 3};
    case 1:
        return {0, 3, 2};
    case 2:
        return {0, 1, 3};
    case 3:
        return {0, 2, 1};
    default:
        return {0, 0, 0};
    }

}


/**
 * @brief Obtiene los índices de los vértices adyacentes a un vértice específico en un prisma.
 * 
 * Dado un índice de vértice dentro de un prisma, esta función devuelve una tupla con los índices de los tres vértices adyacentes.
 * 
 * @param index El índice del vértice para el cual se desean obtener los vértices adyacentes. Debe estar en el rango [0, 5] para un prisma.
 * 
 * @return Una `std::tuple<int, int, int>` que contiene los índices de los tres vértices adyacentes al vértice dado.
 *         Si el índice proporcionado no está en el rango válido, la función devuelve `{0, 0, 0}` como valor predeterminado.
 */
std::tuple<int, int, int> Prism::GetAdjs (int index)
{
    switch(index)
    {
        case 0:
            return {1, 2, 3};
        case 1:
            return {0, 4, 2};
        case 2:
            return {0, 1, 5};
        case 3:
            return {0, 5, 4};
        case 4:
            return {1, 3, 5};
        case 5:
            return {2, 4, 3};
        default:
            return {0, 0, 0};
    }
}


/**
 * @brief Obtiene los índices de los vértices adyacentes a un vértice específico en una piramide.
 * 
 * Dado que la forma de calcular las metricas de una piramide depende del metodo usado es que la funcion esta sin uso.
 */
std::tuple<int, int, int> Pyramid::GetAdjs (int index)
{
    return {0,0,0};
}

void Hexaedron::CalculateJ() 
{
    J.clear();

    for (int index = 0; index < 8; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = (*vertexs_refs[index]).position;;
        glm::vec3 normalized1 = glm::normalize( (*vertexs_refs[adj_index1]).position - origin );
        glm::vec3 normalized2 = glm::normalize( (*vertexs_refs[adj_index2]).position - origin );
        glm::vec3 normalized3 = glm::normalize( (*vertexs_refs[adj_index3]).position - origin );
        J.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }


}
void Hexaedron::CalculateJR() 
{
    JR.clear();
    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }

}
void Hexaedron::CalculateAR()
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
void Hexaedron::CalculateJENS()
{
    Jens.clear();
    Jens = J;
}
void Hexaedron::CalculateEQ()
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
void Hexaedron::CalculateAREN()
{
    ARen = AR;
}

Tetrahedron::Tetrahedron(const std::vector<Vertex*>& vasad) : Polyhedron(vasad)
{
    vertexs_refs = vasad;
}
void Tetrahedron::CalculateJ() 
{

    J.clear();

    for (int index = 0; index < 4; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = (*vertexs_refs[index]).position;;
        glm::vec3 normalized1 = glm::normalize( (*vertexs_refs[adj_index1]).position - origin );
        glm::vec3 normalized2 = glm::normalize( (*vertexs_refs[adj_index2]).position - origin );
        glm::vec3 normalized3 = glm::normalize( (*vertexs_refs[adj_index3]).position - origin );
        J.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }

}
void Tetrahedron::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }

}
void Tetrahedron::CalculateAR()
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
void Tetrahedron::CalculateARG()
{
    float R = 0.0f;

    for (float length: lengths)
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
void Tetrahedron::CalculateJENS()
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
void Tetrahedron::CalculateEQ()
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
void Tetrahedron::CalculateAREN()
{
    ARen = AR;
}   

Pyramid::Pyramid(const std::vector<Vertex*>& vasad) : Polyhedron(vasad)
{
    vertexs_refs = vasad;
}

void Pyramid::CalculateJ() 
{   
    midpoints.clear();
    // Se trata como un hexaedro mal formado, calculando los 4 puntos gaussianos en la puntos de la piramide.
    midpoints.push_back(glm::mix((*vertexs_refs[0]).position, (*vertexs_refs[4]).position, 0.99f));
    midpoints.push_back(glm::mix((*vertexs_refs[1]).position, (*vertexs_refs[4]).position, 0.99f));
    midpoints.push_back(glm::mix((*vertexs_refs[2]).position, (*vertexs_refs[4]).position, 0.99f));
    midpoints.push_back(glm::mix((*vertexs_refs[3]).position, (*vertexs_refs[4]).position, 0.99f));

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
    
    origin = (*vertexs_refs[4]).position;;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[1]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[4]).position;;
    normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[1]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[4]).position;;
    normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[2]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertexs_refs[4]).position;;
    normalizedv1 = glm::normalize( (*vertexs_refs[3]).position - origin );
    normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertexs_refs[0]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    float min_ele = *min_element(values_gauss.begin(), values_gauss.end());
    J.push_back(min_ele);
}

void Polyhedral_Mesh::FixJ(float min_metric, int maxtrys, int method, int selected_metric)
{
    for (auto poly: polyhedrons)
    {
        // EasyFix -> metodo Random
        if (method == 0)
        poly->EasyFix(min_metric, maxtrys, selected_metric);

        else if (method == 1)
        poly->GradFix(min_metric, maxtrys,selected_metric);
    }
}

glm::vec3 Polyhedron::GenerateRandomMove()
{
    std::random_device rd;  // Utilizado para obtener una semilla para el generador de números aleatorios
    std::mt19937 gen(rd()); // Generador de números aleatorios Mersenne Twister
    std::uniform_real_distribution<> dis(-1.0f, 1.0f); // Distribución uniforme en el rango [min, max]

    float x = dis(gen);
    float y = dis(gen);
    float z = dis(gen);

    return glm::normalize(glm::vec3(x, y, z));
}

float Polyhedron::SimulateJ(glm::vec3 new_vertex)
{
    return 0.0f;
}



bool Polyhedron::EasyFix(float t, int maxtrys, int selected_metric) // t: umbral
{
    std::vector<float>* ptrMetrica;

    switch (selected_metric)
    {
    case 0:
        ptrMetrica = &J;
        break;
    case 1:
        ptrMetrica = &JR;
        break;
    case 2:
        ptrMetrica = &Jens;
        break;
    default:
        break;
    }



    // Conseguir todos los vertices bajo el umbral
    // TODO: Ordenar vertices en base a su calidad.
    std::vector <int> indexs_to_fix;
    for (int i = 0; i < (*ptrMetrica).size(); i++)
    {
        if ((*ptrMetrica)[i] < t)
            indexs_to_fix.push_back(i);
    }

    

    // Iterar sobre todos estos vertices
    for (int i = 0; i < indexs_to_fix.size(); i++)
    {
        int actual_try = 0;
        int maxtry = maxtrys;
        
        bool fixed = false;

        // Recalcular Metricas despues de cambiar de vertice
        // Por si el reajuste de otro vertice causo que este fuera arreglado.
        this->CalculateJ();
        this->CalculateJR();
        this->CalculateJENS();


        while(!fixed)
        {
            if((*ptrMetrica)[indexs_to_fix[i]] > t)
            {
                fixed = true;
            }

            glm::vec3 desp = GenerateRandomMove();

            // Delta J
            float bias = 0.005f;
            desp = desp * (std::abs(1.0f - (*ptrMetrica)[indexs_to_fix[i]] ) / 1000.0f + bias);

            glm::vec3 mov = (*vertexs_refs[indexs_to_fix[i]]).position + desp;

            float simulatedmetric = -1.0f;

            switch (selected_metric)
            {
            case 0:
                simulatedmetric = this->SimulateMoveJ(indexs_to_fix[i], mov);
                break;
            case 1:
                simulatedmetric = this->SimulateMoveJR(indexs_to_fix[i], mov);
                break;
            case 2:
                simulatedmetric = this->SimulateMoveJens(indexs_to_fix[i], mov);
                break;
            
            default:
                break;
            }
                
            // Si alcanza el umbral T:
            //      pasar al sgte vertice
            //std::cout << "Proposed move " << mov.x << " " << mov.y << " " << mov.z << " J proposed " << simulatedmetric << std::endl;
            
            if (simulatedmetric > t)
            {
                fixed = true;
                // Mover vertice de indice a la posicion nueva.
                // Pasar al sgte vertice
                (*vertexs_refs[indexs_to_fix[i]]).position.x = mov.x;
                (*vertexs_refs[indexs_to_fix[i]]).position.y = mov.y;
                (*vertexs_refs[indexs_to_fix[i]]).position.z = mov.z;
                (*ptrMetrica)[indexs_to_fix[i]] = simulatedmetric;
            } 
            
            // Si no:
            else {
                
                //      Si mejora con respecto al valor anterior:
                //          Mover hacia el nuevo punto
                if (simulatedmetric > (*ptrMetrica)[indexs_to_fix[i]])
                {
                    (*vertexs_refs[indexs_to_fix[i]]).position.x = mov.x;
                    (*vertexs_refs[indexs_to_fix[i]]).position.y = mov.y;
                    (*vertexs_refs[indexs_to_fix[i]]).position.z = mov.z;
                    (*ptrMetrica)[indexs_to_fix[i]] = simulatedmetric;
                }
                else
                    actual_try++;
                

            }

            // Si se ha alcanzado el numero de intentos maximo:
            // Retornar false
            if(actual_try == maxtry)
            {
                std::cout << "Se alcanzo el intento maximo." << std::endl;
                return false;
            }
        }
        
    }


    return true;
    
    

}



bool Polyhedron::GradFix(float t, int maxtrys, int selected_metric) // t: umbral
{
    std::vector<float>* ptrMetrica;

    switch (selected_metric)
    {
    case 0:
        ptrMetrica = &J;
        break;
    case 1:
        ptrMetrica = &JR;
        break;
    case 2:
        ptrMetrica = &Jens;
        break;
    default:
        break;
    }



    // Conseguir todos los vertices bajo el umbral
    // TODO: Ordenar vertices en base a su calidad.
    std::vector <int> indexs_to_fix;
    for (int i = 0; i < (*ptrMetrica).size(); i++)
    {
        if ((*ptrMetrica)[i] < t)
            indexs_to_fix.push_back(i);
    }

    

    

    // Iterar sobre todos estos vertices
    for (int i = 0; i < indexs_to_fix.size(); i++)
    {
        int actual_try = 0;
        int maxtry = maxtrys;

        float alpha = 0.1;
        float epsilon = 0.1;
        float epsilon_convergencia = 1e-5;


        bool fixed = false;

        // Recalcular Metricas despues de cambiar de vertice
        // Por si el reajuste de otro vertice causo que este fuera arreglado.
        this->CalculateJ();
        this->CalculateJR();
        this->CalculateJENS();


        while(!fixed)
        {
            if((*ptrMetrica)[indexs_to_fix[i]] > t)
            {
                fixed = true;
            }

            glm::vec3 vec_plus_epsilon = (*vertexs_refs[indexs_to_fix[i]]).position + glm::vec3(epsilon);
            glm::vec3 vec_minus_epsilon = (*vertexs_refs[indexs_to_fix[i]]).position - glm::vec3(epsilon);

            float metric_plus, metric_minus;
            
            switch (selected_metric)
            {
            case 0:
                metric_plus = this->SimulateMoveJ(indexs_to_fix[i], vec_plus_epsilon);
                metric_minus = this->SimulateMoveJ(indexs_to_fix[i], vec_minus_epsilon);
                break;
            case 1:
                metric_plus = this->SimulateMoveJR(indexs_to_fix[i], vec_plus_epsilon);
                metric_minus = this->SimulateMoveJR(indexs_to_fix[i], vec_minus_epsilon);
                break;
            case 2:
                metric_plus = this->SimulateMoveJens(indexs_to_fix[i], vec_plus_epsilon);
                metric_minus = this->SimulateMoveJens(indexs_to_fix[i], vec_minus_epsilon);
                break;
            
            default:
                break;
            }

            glm::vec3 grad = (glm::vec3(metric_plus) - glm::vec3(metric_minus)) / (2 * epsilon);

            glm::vec3 x_new = (*vertexs_refs[indexs_to_fix[i]]).position + alpha * grad;

            // Evaluar la métrica

            float simulatedmetric = -1.0f;

            switch (selected_metric)
            {
            case 0:
                simulatedmetric = this->SimulateMoveJ(indexs_to_fix[i], x_new);
                break;
            case 1:
                simulatedmetric = this->SimulateMoveJR(indexs_to_fix[i], x_new);
                break;
            case 2:
                simulatedmetric = this->SimulateMoveJens(indexs_to_fix[i], x_new);
                break;
            
            default:
                break;
            }

            
            // Si alcanza el umbral T:
            //      pasar al sgte vertice
            //std::cout << "Proposed move " << indexs_to_fix[i] << " " << x_new.x << " " << x_new.y << " " << x_new.z << " J proposed " << simulatedmetric << std::endl;

            if (simulatedmetric > t)
            {
                fixed = true;
                // Mover vertice de indice a la posicion nueva.
                // Pasar al sgte vertice
                (*vertexs_refs[indexs_to_fix[i]]).position.x = x_new.x;
                (*vertexs_refs[indexs_to_fix[i]]).position.y = x_new.y;
                (*vertexs_refs[indexs_to_fix[i]]).position.z = x_new.z;
                (*ptrMetrica)[indexs_to_fix[i]] = simulatedmetric;
            } 
            // Si no:
            else {
                
                //      Si mejora con respecto al valor anterior:
                //          Mover hacia el nuevo punto
                if (simulatedmetric > (*ptrMetrica)[indexs_to_fix[i]] + epsilon_convergencia)
                {
                    (*vertexs_refs[indexs_to_fix[i]]).position.x = x_new.x;
                    (*vertexs_refs[indexs_to_fix[i]]).position.y = x_new.y;
                    (*vertexs_refs[indexs_to_fix[i]]).position.z = x_new.z;
                    (*ptrMetrica)[indexs_to_fix[i]] = simulatedmetric;
                }
                else
                {
                    alpha *= 0.9f;
                    actual_try++;
                }


                

            }

            // Si se ha alcanzado el numero de intentos maximo:
            // Retornar false
            if(actual_try == maxtry)
            {
                std::cout << "Se alcanzo el intento maximo." << std::endl;
                return false;
            }
        }
        
    }


    return true;
    
    

}


bool Polyhedron::FixJ(float minJ, int maxtrys)
{
    int actual_try = 0;
    while (actual_try < maxtrys)
    {
        bool fix_needed = false;

        for (int i = 0; i < J.size(); i++)
        {
            if (J[i] < minJ) // TODO:  || J[i] < 0) // Fix si o si
            {
                
                if (dynamic_cast<Hexaedron*>(this))
                {
                    int index_1, index_2, index_3;

                    if (i == 0){
                        index_1 = 1;
                        index_2 = 3;
                        index_3 = 4;
                    } else if (i == 1){
                        index_1 = 0;
                        index_2 = 5;
                        index_3 = 2;
                    } else if (i == 2){
                        index_1 = 1;
                        index_2 = 6;
                        index_3 = 3;
                    } else if (i == 3){
                        index_1 = 0;
                        index_2 = 2;
                        index_3 = 7;
                    } else if (i == 4){
                        index_1 = 0;
                        index_2 = 7;
                        index_3 = 5;
                    } else if (i == 5){
                        index_1 = 1;
                        index_2 = 4;
                        index_3 = 6;
                    } else if (i == 6){
                        index_1 = 2;
                        index_2 = 5;
                        index_3 = 7;
                    } else if (i == 7){
                        index_1 = 3;
                        index_2 = 6;
                        index_3 = 4;
                    } 

                    glm::vec3 vj = glm::normalize((*vertexs_refs[index_1]).position - (*vertexs_refs[i]).position);
                    glm::vec3 vk = glm::normalize((*vertexs_refs[index_2]).position - (*vertexs_refs[i]).position);
                    glm::vec3 vl = glm::normalize((*vertexs_refs[index_3]).position - (*vertexs_refs[i]).position);

                    // Derivadas parciales de J respecto a cada vértice adyacente
                    glm::vec3 dJ_dvj = glm::cross(vk, vl);
                    glm::vec3 dJ_dvk = glm::cross(vl, vj);
                    glm::vec3 dJ_dvl = glm::cross(vj, vk);

                    // Derivadas parciales de vj, vk, vl respecto a la posición del vértice actual
                    glm::vec3 dJ_dvi = dJ_dvj + dJ_dvk + dJ_dvl;

                    (*vertexs_refs[i]).position += dJ_dvi;

                    //std::cout << "Nuevo posicion en " << (*vertexs_refs[i]).position.x << "  " << (*vertexs_refs[i]).position.y << "  " << (*vertexs_refs[i]).position.z << std::endl;
                    //std::cout << "Valor antiguo J " << J[i] << std::endl;

                }
                fix_needed = true;
                break;
            }

            else // No es necesario Fix
                continue; // Avanzar al sgte J_

            
        }

        if (!fix_needed)
            return true;

        actual_try ++;
    }

    return false;




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

float CalculateValueJens(float Js, float k)
{
    if (Js > k)
    {
        return (1 + k) - Js ;
    }
    else if ( -k <= Js && Js <= k )
    {
        return Js / k ;
    }
    else if (Js < -k)
    {
        return -1 * (1 + k) - Js ;
    }
    return 0.0f;
}

void Pyramid::CalculateARG()
{
   
} 
void Pyramid::CalculateJENS()
{

    float kens_ = 0.0f;

    for (int i = 0; i < J.size(); i++)
    {
        float J_ = J[i];


        if (i == 4)
        {
            kens_ = kens_apex;
            glm::vec3 origin, normalizedv1, normalizedv2, normalizedv3;
            std::vector <float> pos;
           
            
            origin = (*vertexs_refs[4]).position;
            normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
            normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
            normalizedv3 = glm::normalize( (*vertexs_refs[1]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            origin = (*vertexs_refs[4]).position;
            normalizedv1 = glm::normalize( (*vertexs_refs[0]).position - origin );
            normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
            normalizedv3 = glm::normalize( (*vertexs_refs[1]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            origin = (*vertexs_refs[4]).position;
            normalizedv1 = glm::normalize( (*vertexs_refs[1]).position - origin );
            normalizedv2 = glm::normalize( (*vertexs_refs[3]).position - origin );
            normalizedv3 = glm::normalize( (*vertexs_refs[2]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            origin = (*vertexs_refs[4]).position;
            normalizedv1 = glm::normalize( (*vertexs_refs[3]).position - origin );
            normalizedv2 = glm::normalize( (*vertexs_refs[2]).position - origin );
            normalizedv3 = glm::normalize( (*vertexs_refs[0]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            bool in_negative = false;
            for(float pos_: pos)
            {
                if(pos_ < 0.0f)
                    in_negative = true;
            }

            if(in_negative)
                Jens.push_back(*max_element(pos.begin(), pos.end()));
            else
                Jens.push_back(*min_element(pos.begin(), pos.end()));



        }
        else 
        {
            kens_ = kens_base;
            Jens.push_back(CalculateValueJens(J_, kens_));
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


Prism::Prism(const std::vector<Vertex*>& vasad) : Polyhedron(vasad)
{
    vertexs_refs = vasad;
}
void Prism::CalculateJ() 
{
    J.clear();

    for (int index = 0; index < 6; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = (*vertexs_refs[index]).position;
        glm::vec3 normalized1 = glm::normalize( (*vertexs_refs[adj_index1]).position - origin );
        glm::vec3 normalized2 = glm::normalize( (*vertexs_refs[adj_index2]).position - origin );
        glm::vec3 normalized3 = glm::normalize( (*vertexs_refs[adj_index3]).position - origin );
        J.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }
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
    for (auto poly: polyhedrons)
    {
        poly->J.clear();
        poly->JR.clear();
        poly->Jens.clear();

        poly->CalculateJ();
        poly->CalculateJR();
        poly->CalculateAR();
        if (std::dynamic_pointer_cast<Tetrahedron>(poly) != nullptr)
            poly->CalculateARG();

        poly->CalculateJENS();
        poly->CalculateEQ();
        poly->CalculateAREN();
    }
    
    // Actualizar Informacion de los graficos
    
    Jtotal.clear();
    Jdata.clear();
    JRdata.clear();
    JENSdata.clear();
    EQdata.clear();


    ARtotal.clear();
    ARdata.clear();
    ARGdata.clear();
    ARENdata.clear();



    for (auto poly: polyhedrons)
    {
        if (std::dynamic_pointer_cast<Hexaedron>(poly) != nullptr)
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
            

        } else if (  std::dynamic_pointer_cast<Tetrahedron>(poly) != nullptr)
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




/**
 * @brief Vincula la información sobre los poliedros a la instancia actual de la clase `Polyhedral_Mesh`.
 * 
 * @param indexs Un `std::vector` de `std::vector<int>`, donde cada `std::vector<int>` contiene los índices de los vértices para un poliedro.
 * @param types Un `std::vector<int>` que contiene el tipo de cada poliedro (por ejemplo, 10 para tetraedros, 12 para hexaedros, etc.).
 */
void Polyhedral_Mesh::BindPolyhedronsInfo(std::vector <std::vector<int>> indexs, std::vector <int> types)
{
    this->indexs = indexs;
    this->types = types;
}


/**
 * @brief Crea poliedros a partir de la información almacenada en los vectores `indexs` y `types`.
 * 
 * Este método utiliza un vector de vértices para construir los poliedros especificados por los vectores `indexs` y `types`.
 * 
 * @param vertices Un `std::vector` de objetos `Vertex` que representa los vértices disponibles para construir los poliedros.
 * 
 * El método realiza las siguientes operaciones:
 * - Itera sobre la información de cada poliedro usando los vectores `indexs` y `types`.
 * - Crea una lista de punteros a los vértices correspondientes para cada poliedro.
 * - Crea una instancia del poliedro correspondiente según el tipo especificado en `types`:
 *   - Tetraedro (tipo 10)
 *   - Hexaedro (tipo 12)
 *   - Prisma (tipo 13)
 *   - Pirámide (tipo 14)
 * - Agrega cada poliedro creado a la lista de poliedros (`polyhedrons`).
 * - Mantiene contadores para el número de cada tipo de poliedro creado (`qtyTetra`, `qtyHexa`, `qtyPrism`, `qtyPyra`).
 * 
 * Nota: Esta función está diseñada para manejar formatos específicos de poliedros. Para añadir más tipos de poliedros en el futuro, se deberá modificar esta función.
 */

std::string Polyhedral_Mesh::MapVertexIndex(int global_index)
{    
    // Buscar la clave en el mapa
    auto it = vertex_to_polyhedron_map.find(global_index);
    
    std::string to_return;

    // Verificar si la clave fue encontrada
    if (it != vertex_to_polyhedron_map.end()) {
        const auto& vec = it->second;
        // Iterar sobre el vector asociado a la clave
        to_return.append("\n");
        for (const auto& pair : vec) {
            // Imprimir cada par en el vector
            //std::cout << "En clave " << key << ": (" << pair.first << ", " << pair.second << ")\n";
            to_return.append("Js: " + std::to_string(polyhedrons[pair.first]->J[pair.second]) + "  P" + std::to_string(pair.first) + "V" + std::to_string(pair.second) + "\n");
            to_return.append("Jens: " + std::to_string(polyhedrons[pair.first]->Jens[pair.second]) + "  P()\n");
        }
    }
    
    return to_return;
}

void Polyhedral_Mesh::CreatePolyhedrons(const std::vector<Vertex>& vertices)
{

    // Iterar sobre la informacion del poliedro i.
    for (int i = 0; i < indexs.size(); i++)
    {
        // Variable temporal para almacenar la direccion de los vertices que seran vinculados.
        std::vector<Vertex*> vertex_refs;

        // Extraer vertices asociados al poliedro actual (i)
        // Se extrae la direccion del vertice que indican el indice j asociados al poliedro i
        for (int j = 0; j < indexs[i].size(); j++)
        {
            vertex_refs.push_back(const_cast<Vertex *>(&vertices[indexs[i][j]]));
            
            // indexs[i][j] Contiene el indice del vertice global.
            // vertex_refs contiene el indice del vertice local.
            vertex_to_polyhedron_map[indexs[i][j]].emplace_back(polyhedrons.size(), vertex_refs.size() - 1);
            
        }
        
        // Crear objeto segun el tipo de poliedro que indique,
        // Dando la referencia de los vertices que le corresponde.
        if (types[i] == 10)
        {
            // Crear un Tetraedro
            polyhedrons.push_back( std::make_shared<Tetrahedron>(vertex_refs) );
            qtyTetra ++;
        }
        else if (types[i] == 12)
        { 
            // Crear un Hexaedro
            polyhedrons.push_back( std::make_shared<Hexaedron>(vertex_refs) );
            qtyHexa ++;
        }
        else if (types[i] == 13)
        {
            // Crear un Prisma
            polyhedrons.push_back( std::make_shared<Prism>(vertex_refs) );
            qtyPrism ++;
        }
        else if (types[i] == 14)
        {
            // Crear una Piramide
            polyhedrons.push_back( std::make_shared<Pyramid>(vertex_refs) );
            qtyPyra ++;
        }     
    }
}


void Polyhedral_Mesh::toString()
{
    std::cout << "\n\nResumen Polyhedros\n N vertices: " << vertices.size() << std::endl; 

    /*
    std::cout << "Imprimiendo vertices" << std::endl;

    for (int i = 0; i < vertices.size(); i++)
    {
        std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
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
        if (std::dynamic_pointer_cast<Tetrahedron>(poly) != nullptr)
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
    */
}
    
std::vector<Tri> Polyhedral_Mesh::toTris()  
{
    std::vector <Tri> converted;

    for (int i = 0; i < indexs.size(); i++)
    {

        switch (types[i])
        {
        case 12:

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
            
            converted.push_back({indexs[i][1], indexs[i][4], indexs[i][0]});
            converted.push_back({indexs[i][1], indexs[i][5], indexs[i][4]});

            converted.push_back({indexs[i][3], indexs[i][0], indexs[i][4]});
            converted.push_back({indexs[i][3], indexs[i][4], indexs[i][7]});
            
            converted.push_back({indexs[i][4], indexs[i][5], indexs[i][6]});
            converted.push_back({indexs[i][4], indexs[i][7], indexs[i][6]});

            converted.push_back({indexs[i][5], indexs[i][1], indexs[i][2]});
            converted.push_back({indexs[i][5], indexs[i][2], indexs[i][6]});

            converted.push_back({indexs[i][2], indexs[i][3], indexs[i][7]});
            converted.push_back({indexs[i][2], indexs[i][6], indexs[i][7]});

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
            converted.push_back({indexs[i][0], indexs[i][1], indexs[i][2]});

            converted.push_back({indexs[i][3], indexs[i][0], indexs[i][1]});
            converted.push_back({indexs[i][3], indexs[i][1], indexs[i][4]});

            converted.push_back({indexs[i][3], indexs[i][4], indexs[i][5]});

            converted.push_back({indexs[i][2], indexs[i][0], indexs[i][3]});
            converted.push_back({indexs[i][2], indexs[i][5], indexs[i][3]});

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


// ------------------------------------------------------------- Metricas individuales.

float Hexaedron::CalculateJ_index(int index)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalized1 = glm::normalize( (*vertexs_refs[adj_index1]).position - (*vertexs_refs[index]).position  );
    glm::vec3 normalized2 = glm::normalize( (*vertexs_refs[adj_index2]).position - (*vertexs_refs[index]).position  );
    glm::vec3 normalized3 = glm::normalize( (*vertexs_refs[adj_index3]).position - (*vertexs_refs[index]).position  );
    return glm::dot( normalized1, glm::cross( normalized2, normalized3));
}

float Hexaedron::SimulateMoveJ(int index, const glm::vec3& mov)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalizedv1 = glm::normalize( (*vertexs_refs[adj_index1]).position - mov );
    glm::vec3 normalizedv2 = glm::normalize( (*vertexs_refs[adj_index2]).position - mov );
    glm::vec3 normalizedv3 = glm::normalize( (*vertexs_refs[adj_index3]).position - mov );

    return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
}

float Hexaedron::CalculateJR_index(int index)
{
    
    float JsMax = *max_element(J.begin(), J.end());

    for(float J_: J)
    {
        JR.push_back(J_ / JsMax);
    }

    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalized1 = glm::normalize( (*vertexs_refs[adj_index1]).position - (*vertexs_refs[index]).position  );
    glm::vec3 normalized2 = glm::normalize( (*vertexs_refs[adj_index2]).position - (*vertexs_refs[index]).position  );
    glm::vec3 normalized3 = glm::normalize( (*vertexs_refs[adj_index3]).position - (*vertexs_refs[index]).position  );
    return glm::dot( normalized1, glm::cross( normalized2, normalized3));
}

float Hexaedron::SimulateMoveJR(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 8; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }

    // Simular JS
    for (int index = 0; index < 8; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = vertexs_mod[index];
        glm::vec3 normalized1 = glm::normalize( vertexs_mod[adj_index1] - origin );
        glm::vec3 normalized2 = glm::normalize( vertexs_mod[adj_index2] - origin );
        glm::vec3 normalized3 = glm::normalize( vertexs_mod[adj_index3] - origin );
        simulatedJ.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }

    // Calcular JR

    float JsMax = *max_element(simulatedJ.begin(), simulatedJ.end());

    return simulatedJ[index] / JsMax;
    
}

float Hexaedron::CalculateJens_index(int index)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalized1 = glm::normalize( (*vertexs_refs[adj_index1]).position - (*vertexs_refs[index]).position  );
    glm::vec3 normalized2 = glm::normalize( (*vertexs_refs[adj_index2]).position - (*vertexs_refs[index]).position  );
    glm::vec3 normalized3 = glm::normalize( (*vertexs_refs[adj_index3]).position - (*vertexs_refs[index]).position  );
    return glm::dot( normalized1, glm::cross( normalized2, normalized3));
    
}

float Hexaedron::SimulateMoveJens(int index, const glm::vec3& mov)
{
    return SimulateMoveJ(index, mov);
}

float Prism::CalculateJ_index(int index)
{
    return 0.0f;
}

float Prism::SimulateMoveJ(int index, const glm::vec3& mov)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalizedv1 = glm::normalize( (*vertexs_refs[adj_index1]).position - mov );
    glm::vec3 normalizedv2 = glm::normalize( (*vertexs_refs[adj_index2]).position - mov );
    glm::vec3 normalizedv3 = glm::normalize( (*vertexs_refs[adj_index3]).position - mov );

    return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));

}

float Prism::CalculateJR_index(int index)
{
    return 0.0f;
}

float Prism::SimulateMoveJR(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 6; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }

    // Simular JS
    for (int index = 0; index < 6; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = vertexs_mod[index];
        glm::vec3 normalized1 = glm::normalize( vertexs_mod[adj_index1] - origin );
        glm::vec3 normalized2 = glm::normalize( vertexs_mod[adj_index2] - origin );
        glm::vec3 normalized3 = glm::normalize( vertexs_mod[adj_index3] - origin );
        simulatedJ.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }

    // Calcular JR

    float JsMax = *max_element(simulatedJ.begin(), simulatedJ.end());

    return simulatedJ[index] / JsMax;
}

float Prism::CalculateJens_index(int index)
{
    return 0.0f;
}

float Prism::SimulateMoveJens(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 6; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }

    // Simular JS
    for (int index = 0; index < 6; index++) 
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = vertexs_mod[index];
        glm::vec3 normalized1 = glm::normalize( vertexs_mod[adj_index1] - origin );
        glm::vec3 normalized2 = glm::normalize( vertexs_mod[adj_index2] - origin );
        glm::vec3 normalized3 = glm::normalize( vertexs_mod[adj_index3] - origin );
        simulatedJ.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }


    if (simulatedJ[index] > kens)
        return (1 + kens) - simulatedJ[index];
    else if ( -kens <= simulatedJ[index] && simulatedJ[index] <= kens )
        return simulatedJ[index] / kens;
    else
        return -1 * (1+kens) - simulatedJ[index];

}


float Pyramid::CalculateJ_index(int index)
{
    return 0.0f;
}

float Pyramid::SimulateMoveJ(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 5; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }


    // Se trata como un hexaedro mal formado, calculando los 4 puntos gaussianos en la puntos de la piramide.
    std::vector <glm::vec3> midpoints_mods;
    midpoints_mods.push_back(glm::mix(vertexs_mod[0], vertexs_mod[4], 0.9f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[1], vertexs_mod[4], 0.9f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[2], vertexs_mod[4], 0.9f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[3], vertexs_mod[4], 0.9f));

    glm::vec3 origin = vertexs_mod[index];
    if (index == 0)
    {
        glm::vec3 normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        glm::vec3 normalizedv2 = glm::normalize( vertexs_mod[3]- origin );
        glm::vec3 normalizedv3 = glm::normalize( midpoints_mods[0] - origin );
        return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
    } else if (index == 1)
    {
        glm::vec3 normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        glm::vec3 normalizedv2 = glm::normalize( midpoints_mods[1] - origin );
        glm::vec3 normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
        return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
    } else if (index == 2)
    {
        glm::vec3 normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        glm::vec3 normalizedv2 = glm::normalize( midpoints_mods[2] - origin );
        glm::vec3 normalizedv3 = glm::normalize( vertexs_mod[3] - origin );
        return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
    } else if (index == 3)
    {
        glm::vec3 normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        glm::vec3 normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        glm::vec3 normalizedv3 = glm::normalize( midpoints_mods[3] - origin );
        return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
    } else
    {
        std::vector <float> values_gauss; 

        glm::vec3 normalizedv1, normalizedv2, normalizedv3; 
        
        origin = vertexs_mod[4];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[0] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
        

        return *min_element(values_gauss.begin(), values_gauss.end());
    }
    



}

float Pyramid::CalculateJR_index(int index)
{
    return 0.0f;
}

float Pyramid::SimulateMoveJR(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 5; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }


    // Calcular JS
    std::vector <glm::vec3> midpoints_mods;
    midpoints_mods.push_back(glm::mix(vertexs_mod[0], vertexs_mod[4], 0.99f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[1], vertexs_mod[4], 0.99f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[2], vertexs_mod[4], 0.99f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[3], vertexs_mod[4], 0.99f));

    glm::vec3 origin, normalizedv1, normalizedv2, normalizedv3;

        origin = vertexs_mod[0];
        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3]- origin );
        normalizedv3 = glm::normalize( midpoints_mods[0] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
    
        origin = vertexs_mod[1];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( midpoints_mods[1] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        origin = vertexs_mod[2];
        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( midpoints_mods[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[3] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
    
        origin = vertexs_mod[3];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( midpoints_mods[3] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        
        std::vector <float> values_gauss; 
        
        origin = vertexs_mod[4];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[0] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
        

    simulatedJ.push_back(*min_element(values_gauss.begin(), values_gauss.end()));


    
    // Calcular JR

    float JsMax = *max_element(simulatedJ.begin(), simulatedJ.end());

    return simulatedJ[index] / JsMax;

}

float Pyramid::CalculateJens_index(int index)
{
    return 0.0f;
}

float Pyramid::SimulateMoveJens(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 5; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }


    // Calcular JS
    std::vector <glm::vec3> midpoints_mods;
    midpoints_mods.push_back(glm::mix(vertexs_mod[0], vertexs_mod[4], 0.99f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[1], vertexs_mod[4], 0.99f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[2], vertexs_mod[4], 0.99f));
    midpoints_mods.push_back(glm::mix(vertexs_mod[3], vertexs_mod[4], 0.99f));

    glm::vec3 origin, normalizedv1, normalizedv2, normalizedv3;

        origin = vertexs_mod[0];
        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3]- origin );
        normalizedv3 = glm::normalize( midpoints_mods[0] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
    
        origin = vertexs_mod[1];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( midpoints_mods[1] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        origin = vertexs_mod[2];
        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( midpoints_mods[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[3] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
    
        origin = vertexs_mod[3];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( midpoints_mods[3] - origin );
    simulatedJ.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        
        std::vector <float> values_gauss; 
        
        origin = vertexs_mod[4];
        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

        normalizedv1 = glm::normalize( vertexs_mod[3] - origin );
        normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
        normalizedv3 = glm::normalize( vertexs_mod[0] - origin );
        values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));
        

    simulatedJ.push_back(*min_element(values_gauss.begin(), values_gauss.end()));


    
    // Calcular Jens

    float kens_ = 0.0f;

        if (index == 4)
        {
            kens_ = kens_apex;
            glm::vec3 origin, normalizedv1, normalizedv2, normalizedv3;
            std::vector <float> pos;
           
            
            origin = vertexs_mod[4];
            normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
            normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
            normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            normalizedv1 = glm::normalize( vertexs_mod[0] - origin );
            normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
            normalizedv3 = glm::normalize( vertexs_mod[1] - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            normalizedv1 = glm::normalize( vertexs_mod[1] - origin );
            normalizedv2 = glm::normalize( vertexs_mod[3] - origin );
            normalizedv3 = glm::normalize( vertexs_mod[2] - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            normalizedv1 = glm::normalize( vertexs_mod[3] - origin );
            normalizedv2 = glm::normalize( vertexs_mod[2] - origin );
            normalizedv3 = glm::normalize( vertexs_mod[0] - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            bool in_negative = false;
            for(float pos_: pos)
            {
                if(pos_ < 0.0f)
                    in_negative = true;
            }

            if(in_negative)
                return *max_element(pos.begin(), pos.end());
            else
                return *min_element(pos.begin(), pos.end());



        }
        else 
        {
            float J_ = simulatedJ[index];
            kens_ = kens_base;
            return CalculateValueJens(J_, kens_);
        }


    
}


float Tetrahedron::CalculateJ_index(int index)
{
    return 0.0f;
}

float Tetrahedron::SimulateMoveJ(int index, const glm::vec3& mov)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalizedv1 = glm::normalize( (*vertexs_refs[adj_index1]).position - mov );
    glm::vec3 normalizedv2 = glm::normalize( (*vertexs_refs[adj_index2]).position - mov );
    glm::vec3 normalizedv3 = glm::normalize( (*vertexs_refs[adj_index3]).position - mov );

    return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
}

float Tetrahedron::CalculateJR_index(int index)
{
    return 0.0f;
}

float Tetrahedron::SimulateMoveJR(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 4; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }

    // Simular JS
    for (int index = 0; index < 4; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = vertexs_mod[index];
        glm::vec3 normalized1 = glm::normalize( vertexs_mod[adj_index1] - origin );
        glm::vec3 normalized2 = glm::normalize( vertexs_mod[adj_index2] - origin );
        glm::vec3 normalized3 = glm::normalize( vertexs_mod[adj_index3] - origin );
        simulatedJ.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }

    // Calcular JR

    float JsMax = *max_element(simulatedJ.begin(), simulatedJ.end());

    return simulatedJ[index] / JsMax;
}

float Tetrahedron::CalculateJens_index(int index)
{
    return 0.0f;
    
}

float Tetrahedron::SimulateMoveJens(int index, const glm::vec3& new_pos)
{
    std::vector <float> simulatedJ;
    std::vector <glm::vec3> vertexs_mod;

    // Posiciones con el vector modificado
    for (int i = 0; i < 4; i++)
    {
        if (i == index)
        {
            glm::vec3 moved = new_pos;
            vertexs_mod.push_back(moved);
        } else {
            vertexs_mod.push_back( (*vertexs_refs[i]).position);
        }
    }

    // Simular JS
    for (int index = 0; index < 4; index++) 
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = vertexs_mod[index];
        glm::vec3 normalized1 = glm::normalize( vertexs_mod[adj_index1] - origin );
        glm::vec3 normalized2 = glm::normalize( vertexs_mod[adj_index2] - origin );
        glm::vec3 normalized3 = glm::normalize( vertexs_mod[adj_index3] - origin );
        simulatedJ.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }


    if (simulatedJ[index] > kens)
        return (1 + kens) - simulatedJ[index];
    else if ( -kens <= simulatedJ[index] && simulatedJ[index] <= kens )
        return simulatedJ[index] / kens;
    else
        return -1 * (1+kens) - simulatedJ[index];
}
