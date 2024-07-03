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
#include <implot.h>

#include <GL/glu.h>
#include <GL/gl.h>

#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <limits>
#include <memory>
#include <unordered_map>
#include <algorithm>




struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color) : position(position), normal(normal), color(color) {}
    Vertex(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) {}
    Vertex(glm::vec3 position) : position(position) {}
};

struct Tri {
    int v0, v1, v2;
};

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;

    BoundingBox() : min(glm::vec3(0.0f, 0.0f, 0.0f)), max(glm::vec3(0.0f, 0.0f, 0.0f)) {}
    BoundingBox(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    bool contains(const glm::vec3& point) const {
        return (point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y &&
                point.z >= min.z && point.z <= max.z);
    }

    bool intersects(const BoundingBox& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x &&
                min.y <= other.max.y && max.y >= other.min.y &&
                min.z <= other.max.z && max.z >= other.min.z);
    }
    
};

/* VTK Formats*/