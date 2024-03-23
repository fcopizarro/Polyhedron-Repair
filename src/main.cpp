#include "common.hpp"
#include "Camera.hpp"

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


// Función para dibujar un cubo
void drawCube(float scale) {
    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo
    glVertex3f(-0.5f, -0.5f , 0.5f);
    glVertex3f(0.5f, -0.5f , 0.5f);
    glVertex3f(0.5f, 0.5f , 0.5f );
    glVertex3f(-0.5f, 0.5f , 0.5f) ;

    // Cara posterior
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Cara izquierda
    glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // Cara derecha
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Cara superior
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Cara inferior
    glColor3f(0.0f, 1.0f, 1.0f); // Cian
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}

// Dimensiones de la ventana
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    
    
    Camera cam;

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

    glEnable(GL_MULTISAMPLE);
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    SDL_GL_SetSwapInterval(1); 



    

        // Crear la ventana
    window = SDL_CreateWindow("SDL VTK Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        return 1;
    }



    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    if (glContext) {
        printf("Se ha creado el contexto GL");
    }


    std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;

    

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
            cam.EventListener(e);
        }

        /*
        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dibujar el contenido del archivo VTK
        drawVTK(renderer);

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
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

        // Reiniciar la transformación de la vista
        

                // Mover la cámara hacia atrás para ver el cubo

        // Rotar la cámara según los ángulos
        
        cam.UpdateCamera();

        // Dibujar el cubo
        drawCube(scale);

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
