#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>


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


// Dimensiones de la ventana
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

// Función para dibujar el contenido del archivo VTK en la pantalla
void drawVTK(SDL_Renderer* renderer) {
    // Aquí deberías leer el archivo VTK y dibujar su contenido en el renderer
    // En este ejemplo, simplemente dibujaremos un cuadrado rojo centrado en la pantalla
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100, 100};
    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Crear la ventana
    window = SDL_CreateWindow("SDL VTK Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Crear el renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Error al crear el renderer: " << SDL_GetError() << std::endl;
        return 1;
    }


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
    int scale = 200;

    for (Point& p : points) {
        p.x = (scale * p.x + screenShift.x);
        p.y = (scale * p.y + screenShift.y);
        p.z = (scale * p.z + screenShift.z);
    }  

    Matrix rotationXYZ = getRotationMatrix();


    // Ciclo principal del juego
    bool quit = false;
    
    while (!quit) {
        SDL_Event e;
        // Manejo de eventos
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
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
        for (Point &p : points) {
            p = translate(screenShiftOpposite, p);
            p = transform(rotationXYZ, p);
            p = translate(screenShift, p);
        }

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
    }

    // Liberar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
