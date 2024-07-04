#include "models.hpp"
#include "polyhedral.hpp"

void search_bb(const OctreeNode* octree, std::vector <BoundingBox> & bb)
{
    if (octree->divided)
    {
        for (auto oct: octree->children)
        {
            search_bb(oct, bb);
        }
    }
    
    if (!octree->divided)
    {
        if (octree->depth == octree->max_depth && octree->to_convert)
        {
            bb.push_back(octree->boundary);
        }
    }
}

Model::Model(const OctreeNode* octree, glm::vec3 color)
{

    std::vector<BoundingBox> bb;
    search_bb(octree, bb);

    int i = 1;

    for(BoundingBox bb_: bb)
    {
        i++;

        int index_ = vertices.size();

        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.min.x, bb_.min.y, bb_.min.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.min.x, bb_.min.y, bb_.max.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.min.x, bb_.max.y, bb_.min.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.max.x, bb_.min.y, bb_.min.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.min.x, bb_.max.y, bb_.max.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.max.x, bb_.max.y, bb_.min.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.max.x, bb_.min.y, bb_.max.z), glm::vec3(0.0f), color));
        vertices.push_back(std::make_shared<Vertex>( glm::vec3(bb_.max.x, bb_.max.y, bb_.max.z), glm::vec3(0.0f), color));


        tris.push_back({index_, index_ + 1, index_ + 2});
        tris.push_back({index_ + 1, index_ + 2, index_ + 4});
        tris.push_back({index_, index_ + 1, index_ + 6});
        tris.push_back({index_, index_ + 3, index_ + 6});
        tris.push_back({index_, index_ + 2, index_ + 3});
        tris.push_back({index_ + 2, index_ + 3, index_ + 5});
        tris.push_back({index_ + 2, index_ + 5, index_ + 7});
        tris.push_back({index_ + 2, index_ + 4, index_ + 7});
        tris.push_back({index_ + 5, index_ + 3, index_ + 6});
        tris.push_back({index_ + 5, index_ + 7, index_ + 6});
        tris.push_back({index_ + 1, index_ + 4, index_ + 6});
        tris.push_back({index_ + 4, index_ + 7, index_ + 6});
        
    }

    BindShader();


    //meshes.push_back(Mesh(vertices, tris, octree->boundary.min,  octree->boundary.max));
}


Model::Model(const std::string& file_, glm::vec3 color)
{
    filename = file_;
    std::cout << filename << std::endl;

    std::string model_ext = obtenerExtension(filename);

    bool model_loaded = true;
    
    if (model_ext == "obj")
    {
        read_obj(filename, color);
        boundary = BoundingBox(min, max);
        
    } else if (model_ext == "vtk")
    {
        read_vtk(filename, color);
        boundary = BoundingBox(min, max);
    }
    else
    {
        model_loaded = false;
        std::cout << "Formato no soportado " << model_ext << std::endl;
    }

    if (model_loaded)
        BindShader();
    




}

void Model::BindShader()
{
    std::vector<Vertex> vertexData;
    for (const auto& vertex : vertices) {
        vertexData.push_back(*vertex);
    }

    calculateFaceNormals(vertexData, tris);

    glGenVertexArrays(1, &ID_VAO);
    glBindVertexArray(ID_VAO);

    glGenBuffers(1, &ID_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    
    glGenBuffers(1, &ID_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(Tri), tris.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Model::calculateFaceNormals(std::vector<Vertex> vertices, const std::vector<Tri>& triangles)
{

    for (const auto& triangle : triangles) {
        glm::vec3& v0 = vertices[triangle.v0].position;
        glm::vec3& v1 = vertices[triangle.v1].position;
        glm::vec3& v2 = vertices[triangle.v2].position;

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        vertices[triangle.v0].normal += normal;
        vertices[triangle.v1].normal += normal;
        vertices[triangle.v2].normal += normal;
    }

    for (auto& vertex : vertices) {
        vertex.normal = glm::normalize(vertex.normal);
    }
}



std::string Model::obtenerExtension(const std::string& nombreArchivo) {
    // Buscar el último punto en el nombre del archivo
    size_t puntoPos = nombreArchivo.find_last_of('.');
    
    // Si no se encontró ningún punto, no hay extensión
    if (puntoPos == std::string::npos) {
        return "";
    }

    // Obtener la extensión a partir del punto
    return nombreArchivo.substr(puntoPos + 1);
}


void Model::setVertexPosition(int index, float x, float y, float z)
{
    vertices[index]->position.x = x;
    vertices[index]->position.y = y;
    vertices[index]->position.z = z;

    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

std::vector<std::shared_ptr<Vertex>> Model::get_vertices()
{
    return vertices;
}

std::vector <Tri> Model::get_tris()
{
    return tris;
}

void updateMinMax(glm::vec3& min, glm::vec3& max, float x, float y, float z)
{

    if (x < min.x)
        min.x = x;

    if (y < min.y)
        min.y = y;

    if (z < min.z)
        min.z = z;

    if (x > max.x)
        max.x = x;

    if (y > max.y)
        max.y = y;

    if (z > max.z)
        max.z = z;

}

void Model::read_obj(const std::string& filename, glm::vec3 color)
{
    std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        }


        std::string line;

        float min_value = std::numeric_limits<float>::lowest();
        float max_value = std::numeric_limits<float>::max();

        glm::vec3 min_ (max_value, max_value, max_value);
        glm::vec3 max_ (min_value, min_value, min_value);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "v") {
                float x, y, z;
                iss >> x >> y >> z;

                updateMinMax(min_, max_, x, y, z);

                Vertex ver = { glm::vec3(x, y, z), glm::vec3(0.0f), color};

                vertices.push_back(std::make_shared<Vertex>(ver));


            } else if (token == "f") {

                std::vector <int> indexs;

                int numero;
                while (!iss.eof()) {
                    if (iss >> numero) {
                        indexs.push_back((GLuint) numero);
                    } else {
                        break;
                    }
                }

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
                    int i4 = indexs.back();
                    indexs.pop_back();

                    int i3 = indexs.back();
                    indexs.pop_back();

                    int i2 = indexs.back();
                    indexs.pop_back();

                    int i1 = indexs.back();
                    indexs.pop_back();

                    /*

                    tris2.push_back(i1 - 1);
                    tris2.push_back(i2 - 1);
                    tris2.push_back(i3 - 1);

                    tris2.push_back(i1 - 1);
                    tris2.push_back(i3 - 1);
                    tris2.push_back(i4 - 1);
                    */

                    Tri tri1 = {i1 - 1, i2 - 1, i3 - 1};
                    Tri tri2 = {i1 - 1, i3 - 1, i4 - 1};

                    tris.push_back(tri1);
                    tris.push_back(tri2);



                } else {
                    std::cout << "Cantidad de caras no soportadas" << std::endl;
                }
                
            }
        }

        file.close();

        std::cout << "Minimo " << min_.x << " " << min_.y << " " << min_.z << " " << std::endl; 
        std::cout << "Maximo " << max_.x<< " "  << max_.y << " " << max_.z << " " << std::endl; 

        min = min_;
        max = max_;
}



void Model::read_vtk(const std::string& filename, glm::vec3 color)
{

    
    float min_value = std::numeric_limits<float>::lowest();
    float max_value = std::numeric_limits<float>::max();

    glm::vec3 min_ (max_value, max_value, max_value);
    glm::vec3 max_ (min_value, min_value, min_value);


    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    }


    std::string line;

    int line_count = 0;

    bool name_getted = false;
    bool cod_getted = false;
    bool dataset_getted = false;
    bool points_getted = false;
    bool cells_getted = false;
    bool cellstype_getted = false;
    std::string structure;



    bool debug = true;

    // leer cabecera.
    while (std::getline(file, line)) {

        if(line.empty())
            continue;


        if (line_count == 1)
        {
            // get name
        }

        if (line_count == 2)
        {
            // get coding
        }

        if (line_count == 3)
        {
            structure = line;
            break;
        }


        if(!line.empty())
            line_count ++;


    }

    if (debug)
        std::cout << "DEBUG: " << line << std::endl;

    std::string points, tipo;
    int points_num;
    
    // Conseguir Encabezado POINTS NUM_PUNTOS TIPO_DE_DATO
    while (!points_getted)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss(line);

        if(line.empty())
            continue;

        iss >> points >> points_num >> tipo;
        std::cout << "Puntos " <<  points_num << std::endl;

        points_getted = true;

        if (debug)
            std::cout << "DEBUG: " << line << std::endl;
    }
    
    std::vector <glm::vec3> points_vector;
    std::vector <float> temp;


    bool points_extracted  = false;


    while (!points_extracted)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss(line);

        float n;
        
        while (iss >> n)
        {
            temp.push_back(n);
        }

        //polyMesh.PushVertex(glm::vec3(x, y, z));

        if ( int(temp.size() / 3) >= points_num)
        {
            points_extracted = true;
            std::cout << "Alcanzado" << std::endl;
        }
    }

    int init = 0;
    for (int i = 0; i < points_num; i += 1)
    {
        updateMinMax(min_, max_, temp[init], temp[init+1], temp[init+2]);
        //vertices.push_back( std::make_shared<Vertex>(glm::vec3(temp[init], temp[init+1], temp[init+2]), glm::vec3(0.0f), glm::vec3(0.5f)));
        polyMesh.PushVertex(glm::vec3(temp[init], temp[init+1], temp[init+2]));
        init +=3;
    }

    min = min_;
    max = max_;
    





    std::string cells;
    int n_cells, n_indexs;

    // Conseguir Encabezado CELLS NUM_CELLS SIZE_INDEXS
    while (!cells_getted)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss_cells(line);

        if(line.empty())
            continue;

        iss_cells >> cells >> n_cells >> n_indexs;

        cells_getted = true;

        if (debug)
            std::cout << "DEBUG: " << line << std::endl;
    }


    

    std::vector <std::vector <int>> total_indexs;


    for (int i = 0; i < n_cells; i++)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss_indexs(line);

        int indexs;

        iss_indexs >> indexs;

        std::vector <int> indices;

        for (int j = 0; j < indexs; j++)
        {
            int i_;
            iss_indexs >> i_;
            indices.push_back(i_);
        }

        total_indexs.push_back(indices);
        polyMesh.PushIndex(indices);

        
    }

    std::cout << "N° indices leidos " << total_indexs.size() << std::endl; 


    std::string cells_types;
    int n_cells_types;

    // Conseguir Encabezado CELLS NUM_CELLS SIZE_INDEXS
    while (!cellstype_getted)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss_cells(line);

        if(line.empty())
            continue;

        iss_cells >> cells_types >> n_cells_types;

        std::cout << line << std::endl;
        cellstype_getted = true;
    }

    std::cout << "NUMERO DE CELDAS " << n_cells_types << std::endl;

    std::vector <int> cells_types_vector;

    for (int i = 0; i < n_cells; i++)
    {
        std::getline(file, line); // cantidad de puntos
        std::istringstream iss_type(line);

        int tipo;

        iss_type >> tipo;

        cells_types_vector.push_back(tipo);
        polyMesh.PushType(tipo);
        
    }

    std::cout << "N° tipos celdas leidos " << cells_types_vector.size() << std::endl; 



    // ------------------------ DATOS EXTRAIDOS ------------------------


    vertices = polyMesh.toVertex();

    polyMesh.FormPolys(vertices);
    polyMesh.CalculateJ();
    polyMesh.GetJ();
    polyMesh.toString(); 

    tris = polyMesh.toTris();

    std::cout << "VER" << vertices.size() << "\n TRI" << tris.size() << std::endl;
    std::cout << vertices[0]->position.x << " " << vertices[0]->position.y << " " << vertices[0]->position.z << std::endl;
    std::cout << vertices[1]->position.x << " " << vertices[1]->position.y << " " << vertices[1]->position.z << std::endl;

    std::cout << tris[0].v0 << " " << tris[0].v1 << " " << tris[0].v2 << std::endl;


    



}

void Model::Draw(Shader& shader, Camera& camera, bool wireframe, bool editmode)
{   
    shader.Activate();

    //Draw_normals();

    glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 2);
    //silhouette();
    /*
    glBindVertexArray(ID_VAO);
    glLineWidth(10.0f);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */

    glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 0);

	glBindVertexArray(ID_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
    glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (editmode)
    {
        glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 1);
        glBindVertexArray(ID_VAO);
        glPointSize(10.0f); // Configurar tamaño del punto
        glDrawArrays(GL_POINTS, 0, vertices.size());

        glBindVertexArray(0);
    }

    

    
}

void Model::silhouette()
{
    glBindVertexArray(ID_VAO);

    glLineWidth(2.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);

    
    glPolygonMode(GL_FRONT, GL_FILL);
    glLineWidth(1.0f);

    glBindVertexArray(0);
}


void Model::Draw_normals()
{   
	glBindVertexArray(vaoLines);
    glDrawArrays(GL_LINES, 0, vertices.size() * 2);
    
    glBindVertexArray(0);
}




