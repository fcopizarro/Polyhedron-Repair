#include "models.hpp"

Model::Model(const std::string& filename)
{
    std::string model_ext = obtenerExtension(filename);
    
    if (model_ext == "obj")
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
                Vertex vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                std::cout << vertex.x << " " <<  vertex.y << " " <<  vertex.z << std::endl;
                vertices.push_back(vertex);
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
                    GLuint i3 = indexs.back();
                    indexs.pop_back();

                    GLuint i2 = indexs.back();
                    indexs.pop_back();

                    GLuint i1 = indexs.back();
                    indexs.pop_back();

                    tris2.push_back(i1);
                    tris2.push_back(i2);
                    tris2.push_back(i3);

                } else if (indexs.size() == 4)
                {
                    GLuint i4 = indexs.back();
                    indexs.pop_back();

                    GLuint i3 = indexs.back();
                    indexs.pop_back();

                    GLuint i2 = indexs.back();
                    indexs.pop_back();

                    GLuint i1 = indexs.back();
                    indexs.pop_back();

                    tris2.push_back(i1 - 1);
                    tris2.push_back(i2 - 1);
                    tris2.push_back(i3 - 1);
                    std::cout << "CARA" << std::endl;
                    std::cout << i1 << std::endl;
                    std::cout << i2 << std::endl;
                    std::cout << i3 << std::endl;

                    tris2.push_back(i1 - 1);
                    tris2.push_back(i3 - 1);
                    tris2.push_back(i4 - 1);
                    std::cout << "CARA" << std::endl;
                    std::cout << i3 << std::endl;
                    std::cout << i4 << std::endl;
                    std::cout << i1 << std::endl;


                } else {
                    std::cout << "Cantidad de caras no soportadas" << std::endl;
                }
                
            }
        }

        file.close();
        meshes.push_back(Mesh(vertices, tris2));
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

void Model::Draw(Shader& shader, Camera& camera)
{
    // Go over all meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		//meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
        meshes[i].Mesh::Draw(shader, camera);
	}
}

void Model::read_obj(const char* file){

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