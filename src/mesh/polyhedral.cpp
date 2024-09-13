#include "polyhedral.hpp"
// Metricas
// {
// vectores y floats de las metricas
// nombre de la metrica
//
//



Polyhedron::Polyhedron(const std::vector<Vertex*>& vertices_ptr){}


// ---------------------- Logica de Hexaedros --------------------------------------

/**
 * Constructor de la clase Hexaedron.
 * 
 * @param vertices_ptr Un vector de punteros a objetos de la clase `Vertex`, que representan los vértices del hexaedro en la Malla.
 */
Hexaedron::Hexaedron(const std::vector<Vertex*>& vertices_ptr) : Polyhedron(vertices_ptr){this->vertices_ptr = vertices_ptr;}

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
 * @brief Calcula el valor de Js para cada vértice del hexaedro.
 *
 * Js se calcula para cada uno de los 8 vértices del hexaedro usando la fórmula:
 * Js = dot(normalized1, cross(normalized2, normalized3)), donde normalized1, normalized2
 * y normalized3 son vectores normalizados entre el vértice actual y los vértices adyacentes.
 */
void Hexaedron::CalculateJs() 
{
    // Limpiar el vector
    Js.clear();

    // Calcular para cada indice 
    for (int index = 0; index < 8; index++) // 8 Vertices de un Hexaedro
    {

        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = (*vertices_ptr[index]).position;;
        glm::vec3 normalized1 = glm::normalize( (*vertices_ptr[adj_index1]).position - origin );
        glm::vec3 normalized2 = glm::normalize( (*vertices_ptr[adj_index2]).position - origin );
        glm::vec3 normalized3 = glm::normalize( (*vertices_ptr[adj_index3]).position - origin );
        Js.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }
}

/* DESACTIVADO
void Hexaedron::CalculateJR() 
{
    JR.clear();
    float JsMax = *max_element(Js.begin(), Js.end());

    for(float J_: Js)
    {
        JR.push_back(J_ / JsMax);
    }

}
*/

 /**
 * @brief Calcula el aspecto de relación (AR) del hexaedro.
 *
 * AR se calcula como la relación entre la distancia mínima y máxima entre vértices del hexaedro.
 * Se obtienen todas las distancias posibles entre pares de vértices y se calcula AR como:
 * AR = min_dist / max_dist.
 */
void Hexaedron::CalculateAR()
{
    float min_dist = std::numeric_limits<float>::max();
    float max_dist = std::numeric_limits<float>::lowest();

    float dist = glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[1]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[2]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;
        
    dist = glm::distance((*vertices_ptr[2]).position, (*vertices_ptr[3]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[3]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[4]).position, (*vertices_ptr[5]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[5]).position, (*vertices_ptr[6]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[6]).position, (*vertices_ptr[7]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[7]).position, (*vertices_ptr[4]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[4]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[5]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[2]).position, (*vertices_ptr[6]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    dist = glm::distance((*vertices_ptr[3]).position, (*vertices_ptr[7]).position);
    if (dist < min_dist)
        min_dist = dist;
    if (dist > max_dist)  
        max_dist = dist;

    AR = min_dist / max_dist;

} 

/**
 * @brief Calcula el valor de Jens.
 *
 * Para el Hexaedro, como la constante es 1.0, entonces Jens = Js.
 */
void Hexaedron::CalculateJENS()
{
    Jens.clear();
    Jens = Js;
}

/*
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
*/

/**
 * @brief Calcula el valor de ARen para un hexaedro.
 * 
 * Como la contantes en un hexaedro es 1.0, entonces el valor de ARen = AR.
 */
void Hexaedron::CalculateAREN()
{
    ARen = AR;
}



// ---------------------- Logica de Tetraedros --------------------------------------


/**
 * Constructor de la clase Tetrahedron.
 * 
 * @param vertices_ptr Un vector de punteros a objetos de la clase `Vertex`, que representan los vértices del hexaedro en la Malla.
 */
Tetrahedron::Tetrahedron(const std::vector<Vertex*>& vertices_ptr) : Polyhedron(vertices_ptr){ this->vertices_ptr = vertices_ptr;}

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
 * @brief Calcula el valor de Js para cada vértice del tetraedro.
 *
 * Js se calcula para cada uno de los 4 vértices del tetraedro usando la fórmula:
 * Js = dot(normalized1, cross(normalized2, normalized3)), donde normalized1, normalized2
 * y normalized3 son vectores normalizados entre el vértice actual y los vértices adyacentes.
 */
void Tetrahedron::CalculateJs() 
{

    Js.clear();

    for (int index = 0; index < 4; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = (*vertices_ptr[index]).position;;
        glm::vec3 normalized1 = glm::normalize( (*vertices_ptr[adj_index1]).position - origin );
        glm::vec3 normalized2 = glm::normalize( (*vertices_ptr[adj_index2]).position - origin );
        glm::vec3 normalized3 = glm::normalize( (*vertices_ptr[adj_index3]).position - origin );
        Js.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }

}

/* DESACTIVADO
void Tetrahedron::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(Js.begin(), Js.end());

    for(float J_: Js)
    {
        JR.push_back(J_ / JsMax);
    }

}
*/

/**
 * @brief Calcula el valor de Jens para cada vértice del tetraedro.
 *
 * Jens se calcula para cada uno de los 8 vértices del hexaedro usando la fórmula que se ve en el codigo.
 * Principalmente busca ajustar las metricas a un nuevo valor.
 */
void Tetrahedron::CalculateJENS()
{

    for (float J_: Js)
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


/**
 * @brief Calcula el aspecto de relación (AR) del tetraedro.
 *
 * AR se calcula como la relación entre la distancia mínima y máxima entre los vértices del tetraedro.
 * El método primero calcula todas las distancias entre pares de vértices y luego determina la distancia
 * mínima y máxima. Finalmente, AR se calcula como:
 * AR = min_dist / max_dist.
 */
void Tetrahedron::CalculateAR()
{
    // Calcular y almacenar las distancias entre todos los pares de vértices del tetraedro
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[1]).position));
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[2]).position));
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[3]).position));
    lengths.push_back(glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[2]).position));
    lengths.push_back(glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[3]).position));
    lengths.push_back(glm::distance((*vertices_ptr[2]).position, (*vertices_ptr[3]).position));

    // Inicializar distancias mínima y máxima
    float min_dist = std::numeric_limits<float>::max();
    float max_dist = std::numeric_limits<float>::lowest();

    // Determinar la distancia mínima y máxima
    for(float length: lengths){

        if (length < min_dist)
            min_dist = length;
        
        if (length > max_dist)
            max_dist = length;
    }

    // Calcular la relación AR y almacenarla
    AR = min_dist / max_dist;
}


/**
 * @brief Calcula el valor de ARG para el tetraedro.
 * 
 * Este método calcula el valor de ARG, una métrica que evalúa la relación entre el radio medio de las aristas del tetraedro y su volumen. 
 * ARG es una medida de calidad que compara la relación entre estas propiedades geométricas del tetraedro.
 * 
 * Los pasos realizados por este método son los siguientes:
 * 1. Calcula el radio medio de las aristas (R) del tetraedro:
 *    - Suma el cuadrado de las longitudes de todas las aristas.
 *    - Divide la suma entre 6 (número de aristas en un tetraedro).
 *    - Toma la raíz cuadrada del resultado para obtener el radio medio.
 * 2. Calcula el volumen (V) del tetraedro usando el producto escalar y el producto cruzado de sus vértices:
 *    - Usa la fórmula para calcular el volumen basado en los vértices del tetraedro.
 * 3. Calcula ARG usando la fórmula:
 *    \[
 *    ARG = \frac{(R^3 \cdot \sqrt{2})}{12 \cdot V}
 *    \]
 * 
 * @note La fórmula para el volumen y el radio medio se basa en las propiedades geométricas del tetraedro.
 */
void Tetrahedron::CalculateARG()
{
    float R = 0.0f;

    for (float length: lengths)
    {
        R += pow(length, 2);
    }

    R /= 6.0f;

    R = sqrt(R);

    // A = (*vertices_ptr[0]).position;
    // B = (*vertices_ptr[1]).position;
    // C = (*vertices_ptr[2]).position;
    // D = (*vertices_ptr[3]).position;

    // abs (glm::dot( B - C, glm::cross( C - A, D - A)) / 6.0f

    float V = abs(glm::dot( (*vertices_ptr[1]).position - (*vertices_ptr[2]).position, glm::cross( (*vertices_ptr[2]).position - (*vertices_ptr[0]).position, (*vertices_ptr[3]).position - (*vertices_ptr[0]).position))) / 6.0f;
    
    ARG = (pow(R, 3) * sqrt(2)) / (12 * V);
} 

/*
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

}*/

/**
 * @brief Calcula el valor de ARen para el tetraedro.
 * 
 * En este caso, el ARen viene a ser el mismo que para el AR.
 */
void Tetrahedron::CalculateAREN()
{
    ARen = AR;
}   



// ---------------------- Logica de Prismas --------------------------------------


/**
 * Constructor de la clase Prism.
 * 
 * @param vertices_ptr Un vector de punteros a objetos de la clase `Vertex`, que representan los vértices del hexaedro en la Malla.
 */
Prism::Prism(const std::vector<Vertex*>& vertices_ptr) : Polyhedron(vertices_ptr) {this->vertices_ptr = vertices_ptr;}

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
 * @brief Calcula el valor de Js para cada vértice del prisma.
 *
 * Js se calcula para cada uno de los 6 vértices del prisma usando la fórmula:
 * Js = dot(normalized1, cross(normalized2, normalized3)), donde normalized1, normalized2
 * y normalized3 son vectores normalizados entre el vértice actual y los vértices adyacentes.
 */
void Prism::CalculateJs() 
{
    Js.clear();

    for (int index = 0; index < 6; index++) // 8 Vertices de un Hexaedro
    {
        auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

        glm::vec3 origin = (*vertices_ptr[index]).position;
        glm::vec3 normalized1 = glm::normalize( (*vertices_ptr[adj_index1]).position - origin );
        glm::vec3 normalized2 = glm::normalize( (*vertices_ptr[adj_index2]).position - origin );
        glm::vec3 normalized3 = glm::normalize( (*vertices_ptr[adj_index3]).position - origin );
        Js.push_back(glm::dot( normalized1, glm::cross( normalized2, normalized3)));        
    }
}

/*
void Prism::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(Js.begin(), Js.end());

    for(float J_: Js)
    {
        JR.push_back(J_ / JsMax);
    }
}
*/

/**
 * @brief Calcula el valor de Jens para cada vértice del prisma.
 *
 * Jens se calcula para cada uno de los 6 vértices del prisma usando la fórmula que se ve en el codigo.
 * Principalmente busca ajustar las metricas a un nuevo valor.
 */
void Prism::CalculateJENS()
{

    
    for (int i = 0; i < Js.size(); i++)
    {
        float J_ = Js[i];

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

/*
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
*/


/**
 * @brief Calcula el aspecto de relación (AR) del prisma.
 *
 * AR se calcula como la relación entre la distancia mínima y máxima entre los vértices del prisma.
 * El método primero calcula todas las distancias entre pares de vértices y luego determina la distancia
 * mínima y máxima. Finalmente, AR se calcula como:
 * AR = min_dist / max_dist.
 */
void Prism::CalculateAR()
{
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[1]).position));
    lengths.push_back(glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[2]).position));
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[2]).position));

    lengths.push_back(glm::distance((*vertices_ptr[3]).position, (*vertices_ptr[4]).position));
    lengths.push_back(glm::distance((*vertices_ptr[4]).position, (*vertices_ptr[5]).position));
    lengths.push_back(glm::distance((*vertices_ptr[3]).position, (*vertices_ptr[5]).position));

    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[3]).position));
    lengths.push_back(glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[4]).position));
    lengths.push_back(glm::distance((*vertices_ptr[2]).position, (*vertices_ptr[5]).position));

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

void Prism::CalculateARG(){}
 
/**
 * @brief Calcula el valor de ARen para el prisma.
 * 
 * El valor se calcula en base a la constante kar asociada al prisma.
 */
void Prism::CalculateAREN()
{
    if (AR > kar)
        ARen = (1+kar) - AR;
    else
        ARen = AR/kar;
}

// ---------------------- Logica de Piramide --------------------------------------


/**
 * Constructor de la clase Pyramid.
 * 
 * @param vertices_ptr Un vector de punteros a objetos de la clase `Vertex`, que representan los vértices del hexaedro en la Malla.
 */
Pyramid::Pyramid(const std::vector<Vertex*>& vertices_ptr) : Polyhedron(vertices_ptr) {this->vertices_ptr = vertices_ptr;}

/**
 * @brief Obtiene los índices de los vértices adyacentes a un vértice específico en una piramide.
 * 
 * Dado que la forma de calcular las metricas de una piramide depende del metodo usado es que la funcion esta sin uso.
 */
std::tuple<int, int, int> Pyramid::GetAdjs (int index)
{
    return {0,0,0};
}

/**
 * @brief Calcula el valor de Js para cada vértice de la piramide.
 *
 * Js se calcula para cada uno de los 5 vértices del piramide usando la fórmula:
 * Js = dot(normalized1, cross(normalized2, normalized3)), donde normalized1, normalized2
 * y normalized3 son vectores normalizados entre el vértice actual y los vértices adyacentes.
 */
void Pyramid::CalculateJs() 
{   
    midpoints.clear();
    // Se trata como un hexaedro mal formado, calculando los 4 puntos gaussianos en la puntos de la piramide.
    midpoints.push_back(glm::mix((*vertices_ptr[0]).position, (*vertices_ptr[4]).position, 0.99f));
    midpoints.push_back(glm::mix((*vertices_ptr[1]).position, (*vertices_ptr[4]).position, 0.99f));
    midpoints.push_back(glm::mix((*vertices_ptr[2]).position, (*vertices_ptr[4]).position, 0.99f));
    midpoints.push_back(glm::mix((*vertices_ptr[3]).position, (*vertices_ptr[4]).position, 0.99f));

    glm::vec3 origin;
    glm::vec3 normalizedv1, normalizedv2, normalizedv3 ;
    
    origin = (*vertices_ptr[0]).position;
    normalizedv1 = glm::normalize( (*vertices_ptr[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertices_ptr[3]).position - origin );
    normalizedv3 = glm::normalize( midpoints[0] - origin );
    Js.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertices_ptr[1]).position;
    normalizedv1 = glm::normalize( (*vertices_ptr[0]).position - origin );
    normalizedv2 = glm::normalize( midpoints[1] - origin );
    normalizedv3 = glm::normalize( (*vertices_ptr[2]).position - origin );
    Js.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertices_ptr[2]).position;
    normalizedv1 = glm::normalize( (*vertices_ptr[1]).position - origin );
    normalizedv2 = glm::normalize( midpoints[2] - origin );
    normalizedv3 = glm::normalize( (*vertices_ptr[3]).position - origin );
    Js.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertices_ptr[3]).position;
    normalizedv1 = glm::normalize( (*vertices_ptr[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertices_ptr[2]).position - origin );
    normalizedv3 = glm::normalize( midpoints[3] - origin );
    Js.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));




    std::vector <float> values_gauss; 
    
    origin = (*vertices_ptr[4]).position;;
    normalizedv1 = glm::normalize( (*vertices_ptr[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertices_ptr[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertices_ptr[1]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertices_ptr[4]).position;;
    normalizedv1 = glm::normalize( (*vertices_ptr[0]).position - origin );
    normalizedv2 = glm::normalize( (*vertices_ptr[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertices_ptr[1]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertices_ptr[4]).position;;
    normalizedv1 = glm::normalize( (*vertices_ptr[1]).position - origin );
    normalizedv2 = glm::normalize( (*vertices_ptr[3]).position - origin );
    normalizedv3 = glm::normalize( (*vertices_ptr[2]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    origin = (*vertices_ptr[4]).position;;
    normalizedv1 = glm::normalize( (*vertices_ptr[3]).position - origin );
    normalizedv2 = glm::normalize( (*vertices_ptr[2]).position - origin );
    normalizedv3 = glm::normalize( (*vertices_ptr[0]).position - origin );
    values_gauss.push_back(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)));

    float min_ele = *min_element(values_gauss.begin(), values_gauss.end());
    Js.push_back(min_ele);
}

/*
void Pyramid::CalculateJR() 
{
    //std::cout << "Calculando JR" << std::endl;

    float JsMax = *max_element(Js.begin(), Js.end());

    for(float J_: Js)
    {
        JR.push_back(J_ / JsMax);
    }

}
*/


/**
 * @brief Calcula el valor de Jens para un vertice especifico.
 */
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

/**
 * @brief Calcula el valor de Jens para cada vértice de la piramide.
 *
 * Jens se calcula para cada uno de los 5 vértices de la piramide usando la fórmula que se ve en el codigo.
 * Principalmente busca ajustar las metricas a un nuevo valor.
 */
void Pyramid::CalculateJENS()
{

    float kens_ = 0.0f;

    for (int i = 0; i < Js.size(); i++)
    {
        float J_ = Js[i];


        if (i == 4)
        {
            kens_ = kens_apex;
            glm::vec3 origin, normalizedv1, normalizedv2, normalizedv3;
            std::vector <float> pos;
           
            
            origin = (*vertices_ptr[4]).position;
            normalizedv1 = glm::normalize( (*vertices_ptr[0]).position - origin );
            normalizedv2 = glm::normalize( (*vertices_ptr[2]).position - origin );
            normalizedv3 = glm::normalize( (*vertices_ptr[1]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            origin = (*vertices_ptr[4]).position;
            normalizedv1 = glm::normalize( (*vertices_ptr[0]).position - origin );
            normalizedv2 = glm::normalize( (*vertices_ptr[3]).position - origin );
            normalizedv3 = glm::normalize( (*vertices_ptr[1]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            origin = (*vertices_ptr[4]).position;
            normalizedv1 = glm::normalize( (*vertices_ptr[1]).position - origin );
            normalizedv2 = glm::normalize( (*vertices_ptr[3]).position - origin );
            normalizedv3 = glm::normalize( (*vertices_ptr[2]).position - origin );
            pos.push_back(CalculateValueJens(glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3)), kens_));

            origin = (*vertices_ptr[4]).position;
            normalizedv1 = glm::normalize( (*vertices_ptr[3]).position - origin );
            normalizedv2 = glm::normalize( (*vertices_ptr[2]).position - origin );
            normalizedv3 = glm::normalize( (*vertices_ptr[0]).position - origin );
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

/**
 * @brief Calcula el aspecto de relación (AR) de la piramide.
 *
 * AR se calcula como la relación entre la distancia mínima y máxima entre los vértices de la piramide.
 * El método primero calcula todas las distancias entre pares de vértices y luego determina la distancia
 * mínima y máxima. Finalmente, AR se calcula como:
 * AR = min_dist / max_dist.
 */
void Pyramid::CalculateAR()
{
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[1]).position));
    lengths.push_back(glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[2]).position));
    lengths.push_back(glm::distance((*vertices_ptr[2]).position, (*vertices_ptr[3]).position));
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[3]).position));
    lengths.push_back(glm::distance((*vertices_ptr[0]).position, (*vertices_ptr[4]).position));
    lengths.push_back(glm::distance((*vertices_ptr[1]).position, (*vertices_ptr[4]).position));
    lengths.push_back(glm::distance((*vertices_ptr[2]).position, (*vertices_ptr[4]).position));
    lengths.push_back(glm::distance((*vertices_ptr[3]).position, (*vertices_ptr[4]).position));

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

void Pyramid::CalculateARG(){}

/*
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
*/

/**
 * @brief Calcula el valor de ARen para la piramide.
 * 
 * El valor se calcula en base a la constante kar asociada a la piramide.
 */
void Pyramid::CalculateAREN()
{
    if (AR > kar)
        ARen = (1+kar) - AR;
    else
        ARen = AR/kar;
}


// ---------------------- Logica de los Poliedros --------------------------------------


/**
 * @brief Genera un vector de movimiento aleatorio.
 * 
 * Este método crea un vector de movimiento aleatorio en 3D. 
 * Utiliza una distribución uniforme para generar valores aleatorios para las coordenadas x, y, y z en el rango de [-1.0, 1.0]. 
 * El vector resultante se normaliza para asegurar que tenga una longitud de 1, es decir, para que sea un vector unitario.
 * 
 * @return Un `glm::vec3` que representa un vector de movimiento aleatorio normalizado.
 */
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


/**
 * @brief Intenta corregir los vértices de un poliedro para mejorar una métrica específica utilizando un método aleatorio.
 * 
 * Este método realiza un ajuste en los vértices del poliedro basándose en una métrica seleccionada. 
 * Para cada vértice cuyo valor de la métrica es menor que el umbral especificado (`t`), el método intenta ajustar su posición mediante movimientos aleatorios. 
 * El ajuste se repite hasta que el valor de la métrica supera el umbral o se alcanzan el número máximo de intentos permitidos (`maxtrys`).
 * 
 * La reparación de los vértices se realiza de manera aleatoria:
 * - Se genera un movimiento aleatorio en el espacio.
 * - Se simula el efecto de este movimiento sobre la métrica seleccionada.
 * - Si el nuevo valor de la métrica supera el umbral, se actualiza la posición del vértice.
 * - Si el nuevo valor no supera el umbral pero mejora la métrica, se actualiza la posición del vértice en función de la mejora.
 * - Si el número máximo de intentos para un vértice se alcanza sin éxito, el método retorna `false`. Si todos los vértices se corrigen exitosamente, retorna `true`.
 * 
 * Se pueden seleccionar diferentes métricas para el ajuste:
 * - Métrica 0: `Js`
 * - Métrica 1: `JR` (actualmente no utilizado en este código)
 * - Métrica 2: `Jens`
 * 
 * @param t Umbral que los valores de la métrica deben superar para considerar que el vértice ha sido corregido.
 * @param maxtrys Número máximo de intentos para corregir cada vértice.
 * @param selected_metric Índice que indica qué métrica se debe utilizar para la corrección (0 para `Js`, 1 para `JR` - no utilizado, 2 para `Jens`).
 * 
 * @return `true` si todos los vértices han sido corregidos exitosamente, `false` si se alcanza el número máximo de intentos para cualquier vértice sin éxito.
 */
bool Polyhedron::EasyFix(float t, int maxtrys, int selected_metric) // t: umbral
{
    std::vector<float>* ptrMetrica;

    switch (selected_metric)
    {
    case 0:
        ptrMetrica = &Js;
        break;
    case 1:
        //ptrMetrica = &JR;
        break;
    case 2:
        ptrMetrica = &Jens;
        break;
    default:
        break;
    }


    // Conseguir todos los vertices bajo el umbral
    // TODO: Permitir que el orden de valores lo haga el usuario (ascendente o descendente)
    std::vector <int> indices_to_fix;
    for (int i = 0; i < (*ptrMetrica).size(); i++)
    {
        if ((*ptrMetrica)[i] < t)
            indices_to_fix.push_back(i);
    }



    // Iterar sobre todos estos vertices
    for (int i = 0; i < indices_to_fix.size(); i++)
    {
        int actual_try = 0;
        int maxtry = maxtrys;
        
        bool fixed = false;

        // Recalcular Metricas despues de cambiar de vertice
        // Por si el reajuste de otro vertice causo que este fuera arreglado.
        this->CalculateJs();
        // this->CalculateJR();
        this->CalculateJENS();


        while(!fixed)
        {
            if((*ptrMetrica)[indices_to_fix[i]] > t)
            {
                fixed = true;
            }

            glm::vec3 desp = GenerateRandomMove();

            // Delta J
            float bias = 0.005f;
            desp = desp * (std::abs(1.0f - (*ptrMetrica)[indices_to_fix[i]] ) / 1000.0f + bias);

            glm::vec3 mov = (*vertices_ptr[indices_to_fix[i]]).position + desp;

            float simulatedmetric = -1.0f;

            switch (selected_metric)
            {
            case 0:
                simulatedmetric = this->SimulateMoveJs(indices_to_fix[i], mov);
                break;
            case 1:
                //simulatedmetric = this->SimulateMoveJR(indices_to_fix[i], mov);
                break;
            case 2:
                simulatedmetric = this->SimulateMoveJens(indices_to_fix[i], mov);
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
                (*vertices_ptr[indices_to_fix[i]]).position.x = mov.x;
                (*vertices_ptr[indices_to_fix[i]]).position.y = mov.y;
                (*vertices_ptr[indices_to_fix[i]]).position.z = mov.z;
                (*ptrMetrica)[indices_to_fix[i]] = simulatedmetric;
            } 
            
            // Si no:
            else {
                
                //      Si mejora con respecto al valor anterior:
                //          Mover hacia el nuevo punto
                if (simulatedmetric > (*ptrMetrica)[indices_to_fix[i]])
                {
                    (*vertices_ptr[indices_to_fix[i]]).position.x = mov.x;
                    (*vertices_ptr[indices_to_fix[i]]).position.y = mov.y;
                    (*vertices_ptr[indices_to_fix[i]]).position.z = mov.z;
                    (*ptrMetrica)[indices_to_fix[i]] = simulatedmetric;
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


/**
 * @brief Intenta corregir los vértices de un poliedro para mejorar una métrica específica utilizando un método basado en gradientes.
 * 
 * Este método ajusta los vértices del poliedro basándose en una métrica seleccionada mediante un enfoque de optimización por gradientes. Para cada vértice cuyo valor de la métrica es menor que el umbral especificado (`t`), el método utiliza una aproximación numérica para calcular el gradiente de la métrica con respecto a la posición del vértice. Luego, ajusta la posición del vértice en la dirección del gradiente para mejorar la métrica. El ajuste se repite hasta que el valor de la métrica supera el umbral o se alcanzan el número máximo de intentos permitidos (`maxtrys`).
 * 
 * El ajuste de los vértices se realiza utilizando el método de gradiente descendente:
 * - Se calcula una aproximación del gradiente numérico mediante diferencias finitas.
 * - Se actualiza la posición del vértice en función del gradiente y un coeficiente de aprendizaje (`alpha`).
 * - Si el nuevo valor de la métrica supera el umbral, se actualiza la posición del vértice.
 * - Si el nuevo valor no supera el umbral pero mejora la métrica, se ajusta la posición del vértice en función de la mejora.
 * - Si no se logra una mejora significativa y se alcanzan el número máximo de intentos, el método ajusta el tamaño del paso (`alpha`).
 * - Si se alcanza el número máximo de intentos para un vértice sin éxito, el método retorna `false`. Si todos los vértices se corrigen exitosamente, retorna `true`.
 * 
 * Se pueden seleccionar diferentes métricas para el ajuste:
 * - Métrica 0: `Js`
 * - Métrica 1: `JR` (actualmente no utilizado en este código)
 * - Métrica 2: `Jens`
 * 
 * @param t Umbral que los valores de la métrica deben superar para considerar que el vértice ha sido corregido.
 * @param maxtrys Número máximo de intentos para corregir cada vértice.
 * @param selected_metric Índice que indica qué métrica se debe utilizar para la corrección (0 para `Js`, 1 para `JR` - no utilizado, 2 para `Jens`).
 * 
 * @return `true` si todos los vértices han sido corregidos exitosamente, `false` si se alcanza el número máximo de intentos para cualquier vértice sin éxito.
 */
bool Polyhedron::GradFix(float t, int maxtrys, int selected_metric) // t: umbral
{
    std::vector<float>* ptrMetrica;

    switch (selected_metric)
    {
    case 0:
        ptrMetrica = &Js;
        break;
    case 1:
        //ptrMetrica = &JR;
        break;
    case 2:
        ptrMetrica = &Jens;
        break;
    default:
        break;
    }



    // Conseguir todos los vertices bajo el umbral
    // TODO: Permitir que el orden de valores lo haga el usuario (ascendente o descendente)
    std::vector <int> indices_to_fix;
    for (int i = 0; i < (*ptrMetrica).size(); i++)
    {
        if ((*ptrMetrica)[i] < t)
            indices_to_fix.push_back(i);
    }
    

    // Iterar sobre todos estos vertices
    for (int i = 0; i < indices_to_fix.size(); i++)
    {
        int actual_try = 0;
        int maxtry = maxtrys;

        float alpha = 0.1;
        float epsilon = 0.1;
        float epsilon_convergencia = 1e-5;


        bool fixed = false;

        // Recalcular Metricas despues de cambiar de vertice
        // Por si el reajuste de otro vertice causo que este fuera arreglado.
        this->CalculateJs();
        //this->CalculateJR();
        this->CalculateJENS();


        while(!fixed)
        {
            if((*ptrMetrica)[indices_to_fix[i]] > t)
            {
                fixed = true;
            }

            glm::vec3 vec_plus_epsilon = (*vertices_ptr[indices_to_fix[i]]).position + glm::vec3(epsilon);
            glm::vec3 vec_minus_epsilon = (*vertices_ptr[indices_to_fix[i]]).position - glm::vec3(epsilon);

            float metric_plus, metric_minus;
            
            switch (selected_metric)
            {
            case 0:
                metric_plus = this->SimulateMoveJs(indices_to_fix[i], vec_plus_epsilon);
                metric_minus = this->SimulateMoveJs(indices_to_fix[i], vec_minus_epsilon);
                break;
            case 1:
                //metric_plus = this->SimulateMoveJR(indices_to_fix[i], vec_plus_epsilon);
                //metric_minus = this->SimulateMoveJR(indices_to_fix[i], vec_minus_epsilon);
                break;
            case 2:
                metric_plus = this->SimulateMoveJens(indices_to_fix[i], vec_plus_epsilon);
                metric_minus = this->SimulateMoveJens(indices_to_fix[i], vec_minus_epsilon);
                break;
            
            default:
                break;
            }

            glm::vec3 grad = (glm::vec3(metric_plus) - glm::vec3(metric_minus)) / (2 * epsilon);

            glm::vec3 x_new = (*vertices_ptr[indices_to_fix[i]]).position + alpha * grad;

            // Evaluar la métrica

            float simulatedmetric = -1.0f;

            switch (selected_metric)
            {
            case 0:
                simulatedmetric = this->SimulateMoveJs(indices_to_fix[i], x_new);
                break;
            case 1:
                //simulatedmetric = this->SimulateMoveJR(indices_to_fix[i], x_new);
                break;
            case 2:
                simulatedmetric = this->SimulateMoveJens(indices_to_fix[i], x_new);
                break;
            
            default:
                break;
            }

            
            // Si alcanza el umbral T:
            //      pasar al sgte vertice
            //std::cout << "Proposed move " << indices_to_fix[i] << " " << x_new.x << " " << x_new.y << " " << x_new.z << " J proposed " << simulatedmetric << std::endl;

            if (simulatedmetric > t)
            {
                fixed = true;
                // Mover vertice de indice a la posicion nueva.
                // Pasar al sgte vertice
                (*vertices_ptr[indices_to_fix[i]]).position.x = x_new.x;
                (*vertices_ptr[indices_to_fix[i]]).position.y = x_new.y;
                (*vertices_ptr[indices_to_fix[i]]).position.z = x_new.z;
                (*ptrMetrica)[indices_to_fix[i]] = simulatedmetric;
            } 
            // Si no:
            else {
                
                //      Si mejora con respecto al valor anterior:
                //          Mover hacia el nuevo punto
                if (simulatedmetric > (*ptrMetrica)[indices_to_fix[i]] + epsilon_convergencia)
                {
                    (*vertices_ptr[indices_to_fix[i]]).position.x = x_new.x;
                    (*vertices_ptr[indices_to_fix[i]]).position.y = x_new.y;
                    (*vertices_ptr[indices_to_fix[i]]).position.z = x_new.z;
                    (*ptrMetrica)[indices_to_fix[i]] = simulatedmetric;
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



// ---------------------- Logica de Malla Poliedrica --------------------------------------

/**
 * @brief Constructor por defecto para la clase Polyhedral_Mesh.
 * 
 * Este constructor no realiza ninguna acción adicional y simplemente inicializa una instancia de la clase `Polyhedral_Mesh` con los valores predeterminados.
 */
Polyhedral_Mesh::Polyhedral_Mesh(){}


/**
 * @brief Ajusta las métricas de calidad de los poliedros en la malla.
 * 
 * Este método itera sobre todos los poliedros en la malla y aplica una de las dos estrategias de corrección disponibles para mejorar las métricas especificadas. La estrategia utilizada se selecciona mediante el parámetro `method`. Dependiendo del valor del parámetro `method`, se emplea el método de corrección aleatorio (EasyFix) o el método de corrección basado en gradientes (GradFix).
 * 
 * @param min_metric El valor mínimo que deben alcanzar las métricas para considerar que los vértices están corregidos. Los vértices con métricas inferiores a este umbral serán corregidos.
 * @param maxtrys El número máximo de intentos permitido para corregir cada vértice. Si se alcanza este número de intentos sin éxito, el método abortará el intento de corrección para ese vértice.
 * @param method Indica el método de corrección a utilizar. Puede ser:
 *               - `0` para el método aleatorio (EasyFix).
 *               - `1` para el método basado en gradientes (GradFix).
 * @param selected_metric Índice que indica qué métrica utilizar para la corrección. Este índice determina cuál de las métricas almacenadas se empleará en la corrección. Puede tener los siguientes valores:
 *                       - `0` para la métrica Js.
 *                       - `1` para la métrica JR (actualmente no implementada).
 *                       - `2` para la métrica Jens.
 * 
 * @return No retorna ningún valor. El método modifica el estado interno de los poliedros en la malla aplicando la corrección según la estrategia seleccionada.
 * 
 * @note Este método llama a `EasyFix` o `GradFix` en cada poliedro dependiendo del valor del parámetro `method`. Asegúrate de que los métodos `EasyFix` y `GradFix` estén implementados correctamente en la clase `Polyhedron`.
 */
void Polyhedral_Mesh::FixJ(float min_metric, int maxtrys, int method, int selected_metric)
{
    for (auto poly: polyhedrons)
    {
        // EasyFix -> metodo Random
        if (method == 0)
        poly->EasyFix(min_metric, maxtrys, selected_metric);

        else if (method == 1)
        poly->GradFix(min_metric, maxtrys, selected_metric);
    }
}



/**
 * @brief Calcula y actualiza las métricas para todos los poliedros en la malla.
 * 
 * Este método recorre todos los poliedros en la malla y calcula varias métricas para cada uno de ellos. Las métricas calculadas incluyen Js, Jens, AR, ARG y AREN. La información calculada se almacena en los vectores correspondientes para su posterior uso o visualización.
 * 
 * Para cada poliedro, el método realiza las siguientes acciones:
 * - Limpia los vectores que almacenan las métricas previas (`Js`, `Jens`, etc.).
 * - Calcula las métricas específicas para cada tipo de poliedro:
 *   - `CalculateJs()`: Calcula la métrica Js.
 *   - `CalculateAR()`: Calcula la métrica AR.
 *   - `CalculateARG()`: Calcula la métrica ARG si el poliedro es un tetraedro.
 *   - `CalculateJENS()`: Calcula la métrica Jens.
 *   - `CalculateAREN()`: Calcula la métrica AREN.
 * 
 * Luego, actualiza los vectores globales que contienen todas las métricas calculadas:
 * - `Jtotal`: Almacena todas las métricas Js, Jens, y cualquier otra métrica relevante.
 * - `Jdata`, `JENSdata`: Almacenan las métricas Js y Jens específicas para cada poliedro.
 * - `ARtotal`: Almacena todas las métricas AR, ARG, y AREN.
 * - `ARdata`, `ARGdata`, `ARENdata`: Almacenan las métricas AR, ARG, y AREN específicas para cada poliedro.
 * 
 * Los poliedros se procesan en función de su tipo:
 * - `Hexaedron`: Si se incluye en el cálculo, se procesan sus métricas Js, Jens, AR, ARG y AREN.
 * - `Tetrahedron`: Si se incluye en el cálculo, se procesan sus métricas Js, Jens, AR, ARG y AREN.
 * - `Pyramid`: Si se incluye en el cálculo, se procesan sus métricas Js, Jens, AR, ARG y AREN.
 * - `Prism`: Si se incluye en el cálculo, se procesan sus métricas Js, Jens, AR, ARG y AREN.
 * 
 * @note Los vectores `JRdata` y `EQdata` están comentados y no se utilizan en el código actual. Si se implementan métricas adicionales, estos vectores deberán ser actualizados en consecuencia.
 */
void Polyhedral_Mesh::CalculateMetrics()
{
    // Limpiar metricas anteriores y actualizar sus valores
    for (auto poly: polyhedrons)
    {
        poly->Js.clear();
        //poly->JR.clear();
        poly->Jens.clear();

        poly->CalculateJs();
        //poly->CalculateJR();
        poly->CalculateAR();
        if (std::dynamic_pointer_cast<Tetrahedron>(poly) != nullptr)
            poly->CalculateARG();

        poly->CalculateJENS();
        //poly->CalculateEQ();
        poly->CalculateAREN();
    }
    
    // Actualizar Informacion para los histogramas
    Jtotal.clear();
    Jdata.clear();
    //JRdata.clear();
    JENSdata.clear();
    //EQdata.clear();

    ARtotal.clear();
    ARdata.clear();
    ARGdata.clear();
    ARENdata.clear();


    // Iterar sobre cada poliedro, evaluando si deben ser incluidos o no, en base a la seleccion en la interfaz de usuario
    for (auto poly: polyhedrons)
    {
        if (std::dynamic_pointer_cast<Hexaedron>(poly) != nullptr)
        {
            if (includeHexa )
            {
                for (float J_: poly->Js)
                    Jdata.push_back(J_);
                //for (float J_: poly->JR)
                //    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                //EQdata.push_back(poly->EQ);
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
                for (float J_: poly->Js)
                    Jdata.push_back(J_);
                //for (float J_: poly->JR)
                //    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                //EQdata.push_back(poly->EQ);
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
                for (float J_: poly->Js)
                    Jdata.push_back(J_);
                //for (float J_: poly->JR)
                //    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                //EQdata.push_back(poly->EQ);
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
                for (float J_: poly->Js)
                    Jdata.push_back(J_);
                //for (float J_: poly->JR)
                //    JRdata.push_back(J_);
                for (float J_: poly->Jens)
                    JENSdata.push_back(J_);

                //EQdata.push_back(poly->EQ);
            }
            if(includePrism2)
            {
                ARdata.push_back(poly->AR);
                ARGdata.push_back(poly->ARG);
                ARENdata.push_back(poly->ARen);
            }

        }
        
    }

    // Formar resumen total de los datos
    for(float J_: Jdata)
        Jtotal.push_back(J_);
    //for(float J_: JRdata)
    //    Jtotal.push_back(J_);
    for(float J_: JENSdata)
        Jtotal.push_back(J_);
    //for(float J_: EQdata)
    //    Jtotal.push_back(J_);


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
 * @param indices Un `std::vector` de `std::vector<int>`, donde cada `std::vector<int>` contiene los índices de los vértices para un poliedro.
 * @param types Un `std::vector<int>` que contiene el tipo de cada poliedro (por ejemplo, 10 para tetraedros, 12 para hexaedros, etc.).
 */
void Polyhedral_Mesh::BindPolyhedronsInfo(std::vector <std::vector<int>> indices, std::vector <int> types)
{
    this->indices = indices;
    this->types = types;
}


/**
 * @brief Crea poliedros a partir de la información almacenada en los vectores `indices` y `types`.
 * 
 * Este método utiliza un vector de vértices para construir los poliedros especificados por los vectores `indices` y `types`.
 * 
 * @param vertices Un `std::vector` de objetos `Vertex` que representa los vértices disponibles para construir los poliedros.
 * 
 * El método realiza las siguientes operaciones:
 * - Itera sobre la información de cada poliedro usando los vectores `indices` y `types`.
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
            to_return.append("Js: " + std::to_string(polyhedrons[pair.first]->Js[pair.second]) + "  P" + std::to_string(pair.first) + "V" + std::to_string(pair.second) + "\n");
            to_return.append("Jens: " + std::to_string(polyhedrons[pair.first]->Jens[pair.second]) + "  P" + std::to_string(pair.first) + "V" + std::to_string(pair.second) + "\n");
        }
    }
    
    return to_return;
}

/**
 * @brief Crea y agrega poliedros a la malla a partir de un conjunto de vértices y sus índices.
 * 
 * Este método toma un vector de vértices y un conjunto de índices que definen poliedros (como tetraedros, hexaedros, prismas y pirámides) y los utiliza para construir los poliedros correspondientes. Los poliedros creados se almacenan en el contenedor `polyhedrons`, y las cantidades de cada tipo de poliedro se actualizan en las variables `qtyTetra`, `qtyHexa`, `qtyPrism` y `qtyPyra`.
 * 
 * El método realiza los siguientes pasos:
 * 1. Itera sobre los índices que definen los poliedros.
 * 2. Para cada poliedro, extrae los vértices correspondientes y los almacena en un vector temporal `vertex_ptr`.
 * 3. Asocia cada vértice global con el poliedro y el índice del vértice local mediante el mapeo en `vertex_to_polyhedron_map`.
 * 4. Crea el poliedro basado en el tipo definido en el vector `types`:
 *    - Si el tipo es 10, se crea un **Tetraedro**.
 *    - Si el tipo es 12, se crea un **Hexaedro**.
 *    - Si el tipo es 13, se crea un **Prisma**.
 *    - Si el tipo es 14, se crea una **Pirámide**.
 * 5. Incrementa la cantidad correspondiente del tipo de poliedro creado.
 * 
 * @param vertices Un vector de vértices que define la geometría global de la malla. Cada vértice contiene la información de su posición y otros atributos necesarios.
 * 
 * @note Asegúrese de que `indices` y `types` estén correctamente definidos y que coincidan con la estructura esperada para evitar errores en la creación de los poliedros.
 */
void Polyhedral_Mesh::CreatePolyhedrons(const std::vector<Vertex>& vertices)
{

    // Iterar sobre la informacion del poliedro i.
    for (int i = 0; i < indices.size(); i++)
    {
        // Variable temporal para almacenar la direccion de los vertices que seran vinculados.
        std::vector<Vertex*> vertex_ptr;

        // Extraer vertices asociados al poliedro actual (i)
        // Se extrae la direccion del vertice que indican el indice j asociados al poliedro i
        for (int j = 0; j < indices[i].size(); j++)
        {
            vertex_ptr.push_back(const_cast<Vertex *>(&vertices[indices[i][j]]));
            
            // Realizar el mapeo desde vertices globales a vertices locales, creados al momento de crear los poliedros
            // indices[i][j] Contiene el indice del vertice global.
            // vertexs_ptr.size() contiene el indice del vertice local.
            // polyhedrons.size() contiene el indice del poliedro que será creado.
            vertex_to_polyhedron_map[indices[i][j]].emplace_back(polyhedrons.size(), vertex_ptr.size() - 1);
            
        }
        
        // Crear objeto segun el tipo de poliedro que indique,
        // Dando la referencia de los vertices que le corresponde.
        if (types[i] == 10)
        {
            // Crear un Tetraedro
            polyhedrons.push_back( std::make_shared<Tetrahedron>(vertex_ptr) );
            qtyTetra ++;
        }
        else if (types[i] == 12)
        { 
            // Crear un Hexaedro
            polyhedrons.push_back( std::make_shared<Hexaedron>(vertex_ptr) );
            qtyHexa ++;
        }
        else if (types[i] == 13)
        {
            // Crear un Prisma
            polyhedrons.push_back( std::make_shared<Prism>(vertex_ptr) );
            qtyPrism ++;
        }
        else if (types[i] == 14)
        {
            // Crear una Piramide
            polyhedrons.push_back( std::make_shared<Pyramid>(vertex_ptr) );
            qtyPyra ++;
        }     
    }
}


/**
 * @brief Convierte los poliedros almacenados en la malla a una lista de triángulos.
 * 
 * Este método transforma los poliedros definidos en la malla en una representación de triángulos. Los triángulos resultantes se almacenan en un vector de triángulos (`Tri`). Cada poliedro se convierte en triángulos basados en su tipo específico:
 * 
 * - **Hexaedro (12 caras):** Se divide en 12 triángulos.
 * - **Tetraedro (10 caras):** Se divide en 4 triángulos.
 * - **Piramide (14 caras):** Se divide en 6 triángulos.
 * - **Prisma (13 caras):** Se divide en 7 triángulos.
 * 
 * El método utiliza el índice de cada vértice para definir las caras de los triángulos y los convierte de acuerdo a las siguientes reglas:
 * 
 * - Para hexaedros, se generan triángulos que cubren todas las caras del poliedro.
 * - Para tetraedros, se generan triángulos que cubren todas las caras del tetraedro.
 * - Para pirámides, se generan triángulos que cubren la base y las caras laterales de la pirámide.
 * - Para prismas, se generan triángulos que cubren las bases y las caras laterales del prisma.
 * 
 * @return Un vector de `Tri` que contiene los triángulos generados a partir de los poliedros.
 * 
 * @note Los tipos de poliedros soportados y sus respectivas conversiones están codificados directamente en el método. Si se encuentran tipos de poliedros no soportados, se imprimirá un mensaje de error en la consola.
 */    
std::vector<Tri> Polyhedral_Mesh::toTris()  
{
    std::vector <Tri> converted;

    for (int i = 0; i < indices.size(); i++)
    {

        switch (types[i])
        {
        case 12:
        // Convertir un Hexaedro en 12 triángulos
        // 0 -> 1 3 4
        // 1 -> 0 2 5
        // 2 -> 1 3 6
        // 3 -> 0 2 7
        // 4 -> 0 5 7
        // 5 -> 1 4 6
        // 6 -> 2 5 7
        // 7 -> 3 4 6
            
            converted.push_back({indices[i][0], indices[i][2], indices[i][1]});
            converted.push_back({indices[i][0], indices[i][3], indices[i][2]});
            
            converted.push_back({indices[i][1], indices[i][4], indices[i][0]});
            converted.push_back({indices[i][1], indices[i][5], indices[i][4]});

            converted.push_back({indices[i][3], indices[i][0], indices[i][4]});
            converted.push_back({indices[i][3], indices[i][4], indices[i][7]});
            
            converted.push_back({indices[i][4], indices[i][5], indices[i][6]});
            converted.push_back({indices[i][4], indices[i][7], indices[i][6]});

            converted.push_back({indices[i][5], indices[i][1], indices[i][2]});
            converted.push_back({indices[i][5], indices[i][2], indices[i][6]});

            converted.push_back({indices[i][2], indices[i][3], indices[i][7]});
            converted.push_back({indices[i][2], indices[i][6], indices[i][7]});

            break;

        case 10:
            // Convertir un Tetraedro en 4 triángulos
            converted.push_back({indices[i][0], indices[i][2], indices[i][1]});
            converted.push_back({indices[i][0], indices[i][1], indices[i][3]});
            converted.push_back({indices[i][1], indices[i][2], indices[i][3]});
            converted.push_back({indices[i][0], indices[i][3], indices[i][2]});

            break;

        case 14:
            // Convertir una Pirámide en 6 triángulos
            converted.push_back({indices[i][0], indices[i][2], indices[i][1]});
            converted.push_back({indices[i][0], indices[i][3], indices[i][2]});
            converted.push_back({indices[i][0], indices[i][1], indices[i][4]});
            converted.push_back({indices[i][1], indices[i][2], indices[i][4]});
            converted.push_back({indices[i][2], indices[i][3], indices[i][4]});
            converted.push_back({indices[i][0], indices[i][4], indices[i][3]});

            break;

        case 13:
            // Convertir un Prisma en 7 triángulos
            converted.push_back({indices[i][0], indices[i][1], indices[i][2]});

            converted.push_back({indices[i][3], indices[i][0], indices[i][1]});
            converted.push_back({indices[i][3], indices[i][1], indices[i][4]});

            converted.push_back({indices[i][3], indices[i][4], indices[i][5]});

            converted.push_back({indices[i][2], indices[i][0], indices[i][3]});
            converted.push_back({indices[i][2], indices[i][5], indices[i][3]});

            converted.push_back({indices[i][1], indices[i][5], indices[i][4]});
            converted.push_back({indices[i][1], indices[i][2], indices[i][5]});

            break;

        
        default:
            std::cout << "Tipo de celda no soportado.\n" << types[i] << std::endl;
            break;
        }
    }
    


    return converted;

}


// ------------------------------------------------------------- Metricas individuales.


/**
 * @brief Calcula el Js para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Js en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */    
float Hexaedron::SimulateMoveJs(int index, const glm::vec3& mov)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalizedv1 = glm::normalize( (*vertices_ptr[adj_index1]).position - mov );
    glm::vec3 normalizedv2 = glm::normalize( (*vertices_ptr[adj_index2]).position - mov );
    glm::vec3 normalizedv3 = glm::normalize( (*vertices_ptr[adj_index3]).position - mov );

    return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
}

/*
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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
*/

/**
 * @brief Calcula el Jens para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Jens en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */   
float Hexaedron::SimulateMoveJens(int index, const glm::vec3& mov)
{
    return SimulateMoveJs(index, mov);
}



/**
 * @brief Calcula el Js para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Js en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */    
float Prism::SimulateMoveJs(int index, const glm::vec3& mov)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalizedv1 = glm::normalize( (*vertices_ptr[adj_index1]).position - mov );
    glm::vec3 normalizedv2 = glm::normalize( (*vertices_ptr[adj_index2]).position - mov );
    glm::vec3 normalizedv3 = glm::normalize( (*vertices_ptr[adj_index3]).position - mov );

    return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));

}

/*
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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
*/

/**
 * @brief Calcula el Jens para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Jens en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */   
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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



/**
 * @brief Calcula el Js para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Js en caso de que el vertice index sea movido al punto de mov.
 * Para el caso de la cuspide se selecciona la metrica con menor valor .
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */    
float Pyramid::SimulateMoveJs(int index, const glm::vec3& new_pos)
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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

/*
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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
*/

/**
 * @brief Calcula el Jens para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Jens en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */   
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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



/**
 * @brief Calcula el Js para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Js en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */    
float Tetrahedron::SimulateMoveJs(int index, const glm::vec3& mov)
{
    auto [adj_index1, adj_index2, adj_index3] = GetAdjs(index);

    glm::vec3 normalizedv1 = glm::normalize( (*vertices_ptr[adj_index1]).position - mov );
    glm::vec3 normalizedv2 = glm::normalize( (*vertices_ptr[adj_index2]).position - mov );
    glm::vec3 normalizedv3 = glm::normalize( (*vertices_ptr[adj_index3]).position - mov );

    return glm::dot( normalizedv1, glm::cross( normalizedv2, normalizedv3));
}

/* DESHABILITADO
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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
*/

/**
 * @brief Calcula el Jens para un vertice en especifico con un desplazamiento en el espacio del mismo vertice.
 * 
 * Simula el valor de Jens en caso de que el vertice index sea movido al punto de mov.
 * 
 * @param index: indice del vertice a calcular.
 * @param mov: vector con la posicion simulada del vertice en el espacio.
 */   
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
            vertexs_mod.push_back( (*vertices_ptr[i]).position);
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
