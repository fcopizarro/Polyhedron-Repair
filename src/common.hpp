#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GL/glew.h>

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#include <GL/glu.h>
#include <GL/gl.h>

#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Tri {
    int v0, v1, v2;
};


struct Face {
    int v1, v2, v3;
};

struct Face2 {
    GLuint v1, v2, v3;
};