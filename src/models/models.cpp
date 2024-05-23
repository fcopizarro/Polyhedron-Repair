#include "models.hpp"

Model::Model(const std::string& filename)
{
    std::string model_ext = obtenerExtension(filename);
    
    if (model_ext == "obj")
    {
        read_obj(filename);
    }
    else
    {
        std::cout << "Formato no soportado " << model_ext << std::endl;
    }
    /*
	// Make a JSON object
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	// Get the binary data
	Model::file = file;
	data = getData();

	// Traverse all nodes
	traverseNode(0);
    */
}

void Model::read_obj(const std::string& filename)
{
    std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        }


        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "v") {
                float x, y, z;
                iss >> x >> y >> z;

                Vertex ver = { glm::vec3(x, y, z), glm::vec3(0.0f) };

                vertices.push_back(ver);


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
        meshes.push_back(Mesh(vertices, tris));
}

void Model::Draw(Shader& shader, Camera& camera)
{
    // Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		//meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
        meshes[i].Mesh::Draw(shader, camera);
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