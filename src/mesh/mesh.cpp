#include "mesh.hpp"
#include "polyhedral.hpp"




Mesh::Mesh(const std::string& filename)
{
    
    // Instanciar lector y darle el archivo deseado.
    Reader reader;
    reader.read_file(filename);
    
    if (reader.read_status)
    {
      // Vertices de la malla que son usados en la visualizacion
      vertices = reader.vertices;
      
      if (reader.has_polyhedrons)
      {
        // Creacion de la version Polihedrica de los datos.
        
        // Vincular Indices y Tipos de Celdas leidos
        polyMesh.BindPolyhedronsInfo(reader.indices, reader.cells_types_vector);
        // Crear Polihedros
        polyMesh.CreatePolyhedrons(vertices);
        // Calcular metricas una vez inicializado la malla de poliedros
        //polyMesh.CalculateMetrics();
        // Convertir superficie de poliedros a tris.

        /*
        for (auto &indice: reader.indices)
            polyMesh.PushIndex(indice);
            
        polyMesh.types = reader.cells_types_vector;
        polyMesh.FormPolys(vertices);
        //polyMesh.CalculateJ();
        //polyMesh.GetJ();
        */
        tris = polyMesh.toTris();
        polyMesh.CalculateJ();
      } else
      {
        tris = reader.tris;
      }
      
      
      BindShader();
      
    }

// TODO: Checkear vertices mayores y menores al final....
// TODO: Graficar lineas de tris

}

glm::vec3 Mesh::GetCenter()
{
    return glm::vec3( (boundary.min.x + boundary.max.x) / 2.0f, (boundary.min.y + boundary.max.y) / 2.0f, (boundary.min.z + boundary.max.z) / 2.0f  );
}


void Mesh::BindShader()
{

    calculateFaceNormals(vertices, tris);

    glGenVertexArrays(1, &ID_VAO);
    glBindVertexArray(ID_VAO);

    glGenBuffers(1, &ID_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);


    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    
    glGenBuffers(1, &ID_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size() * sizeof(Tri), tris.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //renderNormals(vertices);
    BindMeshLines();
}


void Mesh::calculateFaceNormals(std::vector<Vertex>& vertices, const std::vector<Tri>& triangles)
{
    for (auto& vertex : vertices) {
        vertex.normal = glm::vec3(0.0f);
        vertex.count = 0;
    }

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

        vertices[triangle.v0].count++;
        vertices[triangle.v1].count++;
        vertices[triangle.v2].count++;


    }

    for (auto& vertex : vertices) {
        if (vertex.count > 0) {
            vertex.normal = glm::normalize(vertex.normal / static_cast<float>(vertex.count));
        }
    }
}




void Mesh::setVertexPosition(int index, float x, float y, float z)
{
    vertices[index].position = glm::vec3(x, y, z);

    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::UpdateModelGraph()
{
    
    glBindBuffer(GL_ARRAY_BUFFER, ID_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector <Vertex> Mesh::get_vertices()
{
    return vertices;
}

std::vector <Tri> Mesh::get_tris()
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


void Mesh::Draw(Shader& shader, Camera& camera, bool editmode, int selected_vertex)
{   
    shader.Activate();
    glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 2);
    Draw_normals();
    DrawMeshLines();

    glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 0);

    glBindVertexArray(ID_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID_EBO);
    glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    if (editmode)
    {
        glPointSize(10.0f); // Configurar tamaño del punto
        glBindVertexArray(ID_VAO);
      
        if (selected_vertex != -1)
        {
          // Cambiar color del vertice seleccionado
          glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 3);
          glDrawArrays(GL_POINTS, selected_vertex, 1);
          
          glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 1);
          glDrawArrays(GL_POINTS, 0, vertices.size());
        
        } else {
          glUniform1i(glGetUniformLocation(shader.ID, "isVertex"), 1);
          glDrawArrays(GL_POINTS, 0, vertices.size());
        }
        glBindVertexArray(0);   
    }
}

void Mesh::silhouette()
{
    glBindVertexArray(ID_VAO);

    glLineWidth(2.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, tris.size() * 3, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glLineWidth(1.0f);

    glBindVertexArray(0);
}

void Mesh::renderNormals(const std::vector<Vertex>& vertices) {
    
    /*
    std::vector<glm::vec3> lineVertices2;
    for (const auto& vertex : vertices) {
        lineVertices2.push_back(vertex.position);
        lineVertices2.push_back(vertex.position + vertex.normal * 0.1f); // Escalar normal para visualización
    }

    glGenVertexArrays(1, &vaoLines);
    glBindVertexArray(vaoLines);

    

    glGenBuffers(1, &vboLineVertices2);

    glBindBuffer(GL_ARRAY_BUFFER, vboLineVertices2);

    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(glm::vec3), lineVertices.data(), GL_DYNAMIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(0); // Desenlazar VAO
    */
}

void Mesh::updateNormals() {
    std::vector<glm::vec3> lineVertices2;
    for (const auto& vertex : vertices) {
        lineVertices2.push_back(vertex.position);
        lineVertices2.push_back(vertex.position + vertex.normal * 0.1f); // Escalar normal para visualización
    }

    glBindVertexArray(vaoLines);
    glBindBuffer(GL_ARRAY_BUFFER, vboLineVertices);
    glBufferData(GL_ARRAY_BUFFER, lineVertices2.size() * sizeof(glm::vec3), lineVertices2.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0); // Desenlazar VAO
}


void Mesh::Draw_normals()
{   
	glBindVertexArray(vaoLines);
    glDrawArrays(GL_LINES, 0, vertices.size() * 2);
    
    glBindVertexArray(0);
}

void Mesh::BindMeshLines()
{
    lineVertices.clear();
    // TODO: ELIMINAR USO DE POLYS, HACER QUE LA LLAMDA SEA DIRECTA A LA CLASE Y NO USAR ELEMENTOS DE LA CLASE.
    for (auto poly : polyMesh.polyhedrons) {

        if (std::dynamic_pointer_cast<Hexaedron>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            
            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[4]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);

            lineVertices.push_back((*poly->vertexs_refs[5]).position);
            lineVertices.push_back((*poly->vertexs_refs[6]).position);

            lineVertices.push_back((*poly->vertexs_refs[6]).position);
            lineVertices.push_back((*poly->vertexs_refs[7]).position);

            lineVertices.push_back((*poly->vertexs_refs[4]).position);
            lineVertices.push_back((*poly->vertexs_refs[7]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[6]).position);

            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[7]).position);
        }
        else if (std::dynamic_pointer_cast<Tetrahedron>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[0]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);
        }
        else if (std::dynamic_pointer_cast<Pyramid>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);
        }
        else if (std::dynamic_pointer_cast<Prism>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);



            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[4]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);

            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);


        }
        //lineVertices.push_back(vertex.position);
        //lineVertices.push_back(vertex.position + vertex.normal * 0.1f); // Escalar normal para visualización
    }



    glGenVertexArrays(1, &vaoMeshLines);
    glGenBuffers(1, &vboMeshLines);
    glBindVertexArray(vaoMeshLines);


    glBindBuffer(GL_ARRAY_BUFFER, vboMeshLines);
    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(glm::vec3), lineVertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::UpdateMeshLines()
{
    lineVertices.clear();
    
    for (auto poly : polyMesh.polyhedrons) {

        if (std::dynamic_pointer_cast<Hexaedron>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            
            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[4]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);

            lineVertices.push_back((*poly->vertexs_refs[5]).position);
            lineVertices.push_back((*poly->vertexs_refs[6]).position);

            lineVertices.push_back((*poly->vertexs_refs[6]).position);
            lineVertices.push_back((*poly->vertexs_refs[7]).position);

            lineVertices.push_back((*poly->vertexs_refs[4]).position);
            lineVertices.push_back((*poly->vertexs_refs[7]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[6]).position);

            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[7]).position);
        }
        else if (std::dynamic_pointer_cast<Tetrahedron>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[0]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);
        }
        else if (std::dynamic_pointer_cast<Pyramid>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);
        }
        else if (std::dynamic_pointer_cast<Prism>(poly) != nullptr)
        {
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[1]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[2]).position);

            
            lineVertices.push_back((*poly->vertexs_refs[0]).position);
            lineVertices.push_back((*poly->vertexs_refs[3]).position);

            lineVertices.push_back((*poly->vertexs_refs[1]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[2]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);



            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[4]).position);

            lineVertices.push_back((*poly->vertexs_refs[4]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);

            lineVertices.push_back((*poly->vertexs_refs[3]).position);
            lineVertices.push_back((*poly->vertexs_refs[5]).position);


        }
    }
    

    glBindBuffer(GL_ARRAY_BUFFER, vboMeshLines);
    glBufferSubData(GL_ARRAY_BUFFER, 0, lineVertices.size() * sizeof(glm::vec3), lineVertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void Mesh::DrawMeshLines()
{
    glBindVertexArray(vaoMeshLines);
    glLineWidth(2.0f);
    //std::cout << "CANTIDAD DE VERTICES" << vertices.size() << std::endl;
    glDrawArrays(GL_LINES, 0, lineVertices.size());
    
    glBindVertexArray(0);
}




