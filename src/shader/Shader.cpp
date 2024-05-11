#include "shader/Shader.hpp"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	} else{
        std::cerr << "Error: " << strerror(errno) << std::endl;


    }
	throw(errno);
}



GLuint Shader::loadShader(const char *shaderSource, GLenum shaderType) {
    GLuint shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }

    return shaderID;
}

Shader::Shader () 
{
    std::string vertexCode = get_file_contents("../src/shader/default/vertex.glsl");
	std::string fragmentCode = get_file_contents("../src/shader/default/fragment.glsl");

    GLuint vertexShader = loadShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    // Crear un programa de shader y adjuntar los shaders compilados
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    GLint success;
    char infoLog[1024];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "Error al enlazar el programa de shader:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}

