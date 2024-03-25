
#include "common.hpp"
#include "Camera.hpp"
#include <string>
#include <iostream>



class Point {
public:
    double x, y, z;

    Point() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point(double x, double y) {
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    double operator[](int i) const {
        if (i == 0) return this->x;
        if (i == 1) return this->y;
        return this->z;
    }

    double& operator[](int i) {
        if (i == 0) return this->x;
        if (i == 1) return this->y;
        return this->z;
    }
};

const GLchar* vertexShaderSource = R"glsl(

    #version 330 core
    layout (location = 0) in vec3 aPos;  // La posición del vértice
    uniform mat4 model;  // Matriz de modelo
    uniform mat4 view;   // Matriz de vista
    uniform mat4 projection; // Matriz de proyección
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }


)glsl";

const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)glsl";



typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

Matrix dot(const Matrix& a, const Matrix& b) {
    Matrix result = Matrix(a.size(), Vector(b[0].size(), 0));
    for (int i=0; i<a.size(); i++) {
        for (int j=0; j<b[0].size(); j++) {
            for (int k=0; k<b.size(); k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

Point transform(const Matrix& matrix, const Point& point) {
    Matrix p = {{point.x}, {point.y}, {point.z}};
    Matrix r = dot(matrix, p);
    return Point(r[0][0], r[1][0], r[2][0]);
}

Point translate(const Point& shift, const Point& point) {
    return Point(
        point.x + shift.x,
        point.y + shift.y,
        point.z + shift.z
    );
}

void connect(SDL_Renderer* const renderer, const std::vector<Point> &points, int i, int j) {
    SDL_RenderDrawLine(
        renderer,
        points[i].x,
        points[i].y,
        points[j].x,
        points[j].y
    );
}

Matrix getRotationMatrix() {
    double alpha = 0.001;
    Matrix rotationX = {
        {1, 0, 0},
        {0, cos(alpha), -sin(alpha)},
        {0, sin(alpha), cos(alpha)}
    };

    double beta = 0.002;
    Matrix rotationY = {
        {cos(beta), 0, sin(beta)},
        {0, 1, 0},
        {-sin(beta), 0, cos(beta)}
    };

    double gamma = 0.003;
    Matrix rotationZ = {
        {cos(gamma), -sin(gamma), 0},
        {sin(gamma), cos(gamma), 0},
        {0, 0, 1}
    };

    return dot(rotationZ, dot(rotationY, rotationX));
}

void drawCube() {
    /*
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,  // Vértice 1 (inferior izquierda)
        0.5f, -0.5f, 0.0f,  // Vértice 2 (inferior derecha)
        0.0f,  0.5f, 0.0f   // Vértice 3 (superior centro)
    };

    GLuint indices[] = {
        0, 1, 2  // Índices de los vértices que forman el triángulo
    };
    */

   GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};
/*
   
    GLfloat vertices[] = {
        -1.0f, -1.0f,  1.0f, // Front bottom left
         1.0f, -1.0f,  1.0f, // Front bottom right
         1.0f,  1.0f,  1.0f, // Front top right
        -1.0f,  1.0f,  1.0f, // Front top left
         1.0f, -1.0f, -1.0f, // Back bottom right
        -1.0f, -1.0f, -1.0f, // Back bottom left
        -1.0f,  1.0f, -1.0f, // Back top left
         1.0f,  1.0f, -1.0f  // Back top right

    };          // 8 of vertex coords
    GLubyte indices[] = {0,1,2, 2,3,0,   // 36 of indices
                        0,3,4, 4,5,0,
                        0,5,6, 6,1,0,
                        1,6,7, 7,2,1,
                        7,4,3, 3,2,7,
                        4,7,6, 6,5,4};*/

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);


}


// Dimensiones de la ventana
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    
    
    Camera cam (SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, -1.0f));

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;


    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }


    

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 





    

        // Crear la ventana
    window = SDL_CreateWindow("SDL VTK Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        return 1;
    }



    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Error al crear el contexto OpenGL: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cerr << "Error al inicializar SDL2: " << SDL_GetError() << std::endl;

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }



    SDL_GL_SetSwapInterval(1); 

    SDL_GL_MakeCurrent(window, glContext);
    
    glEnable(GL_MULTISAMPLE);
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );



    

    int buffers, samples;
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &buffers);
    SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &samples);
    printf("Multisample Buffers: %d, Multisample Samples: %d\n", buffers, samples);

    

    /* Setup our viewport. */
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    double ratio = (double) SCREEN_WIDTH /(double) SCREEN_HEIGHT;
    //gluPerspective( 60.0, 1.0 / ratio, 1.0, 100.0 );
    glOrtho(-1.0f, 1.0f, -1.0f / ratio, 1.0f / ratio, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    



    std::vector<Point> points = {
        Point(-1, 1, 1),
        Point(1, 1, 1),
        Point(1, -1, 1),
        Point(-1, -1, 1),
        Point(-1, 1, -1),
        Point(1, 1, -1),
        Point(1, -1, -1),
        Point(-1, -1, -1)
    };


    Point screenShift(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    Point screenShiftOpposite(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2);
    float scale = 100;

    for (Point& p : points) {
        p.x = (scale * p.x + screenShift.x);
        p.y = (scale * p.y + screenShift.y);
        p.z = (scale * p.z + screenShift.z);
    }  

    Matrix rotationXYZ = getRotationMatrix();





    // Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Checks if Shader compiled succesfully
	//compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Checks if Shader compiled succesfully
	//compileErrors(fragmentShader, "FRAGMENT");



    // Crear un programa de shader y adjuntar los shaders compilados
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    char infoLog[1024];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error al enlazar el programa de shader:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Ciclo principal del juego
    bool quit = false;
    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);

        
    while (!quit) {
        SDL_Event e;
        // Manejo de eventos
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            cam.Inputs(e);
        }

        /*
        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dibujar el contenido del archivo VTK
        drawVTK(renderer);

        */
        //for (Point &p : points) {
        //    p = translate(screenShiftOpposite, p);
            //p = transform(rotationXYZ, p);
        //    p = translate(screenShift, p);
        //}

        /*

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i=0; i<4; i++) {
            connect(renderer, points, i, (i + 1) % 4);
            connect(renderer, points, i + 4, ((i + 1) % 4) + 4);
            connect(renderer, points, i, i + 4);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(3);
        */
       // Limpiar pantalla y buffer de profundidad
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR) {
            std::cerr << err << std::endl;        
        }

        
        glUseProgram(shaderProgram);

        cam.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");


        // Reiniciar la transformación de la vista
        

                // Mover la cámara hacia atrás para ver el cubo

        // Rotar la cámara según los ángulos
        
        // Dibujar el cubo
        drawCube();

        //glTranslatef(0.0f, 0.0f, -5.0f);


        // Intercambiar buffers
        SDL_GL_SwapWindow(window);
    }

    // Liberar recursos
    //SDL_DestroyRenderer(renderer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
