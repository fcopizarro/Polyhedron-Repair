#include "octree.hpp"

bool OctreeNode::subdivide(const std::vector <Vertex> vertices, std::vector <Tri> tris)
{
    // Subdividir si o si a un nivel
    // TODO: subdividir solo si el bounding box resultante es distinto al padre.
    if (!(depth <= max_depth))
        return false;


    // if depth == max_depth => is_displayed = true
    




    if (is_divisible(vertices, tris))
    {
        if (depth == max_depth)
        {
            to_convert = true;
            return false;
        }

        //std::cout << "Se puede dividir" << std::endl;

        float midX = (boundary.min.x + boundary.max.x) / 2.0f;
        float midY = (boundary.min.y + boundary.max.y) / 2.0f;
        float midZ = (boundary.min.z + boundary.max.z) / 2.0f;

        glm::vec3 min = boundary.min;
        glm::vec3 max = boundary.max;


        children[0] = new OctreeNode(BoundingBox(min, glm::vec3(midX, midY, midZ)), depth + 1, max_depth);
        children[1] = new OctreeNode(BoundingBox(glm::vec3(midX, min.y, min.z), glm::vec3(max.x, midY, midZ)), depth + 1, max_depth);
        children[2] = new OctreeNode(BoundingBox(glm::vec3(min.x, midY, min.z), glm::vec3(midX, max.y, midZ)), depth + 1, max_depth);
        children[3] = new OctreeNode(BoundingBox(glm::vec3(midX, midY, min.z), glm::vec3(max.x, max.y, midZ)), depth + 1, max_depth);
        children[4] = new OctreeNode(BoundingBox(glm::vec3(min.x, min.y, midZ), glm::vec3(midX, midY, max.z)), depth + 1, max_depth);
        children[5] = new OctreeNode(BoundingBox(glm::vec3(midX, min.y, midZ), glm::vec3(max.x, midY, max.z)), depth + 1, max_depth);
        children[6] = new OctreeNode(BoundingBox(glm::vec3(min.x, midY, midZ), glm::vec3(midX, max.y, max.z)), depth + 1, max_depth);
        children[7] = new OctreeNode(BoundingBox(glm::vec3(midX, midY, midZ), glm::vec3(max.x, max.y, max.z)), depth + 1, max_depth);

        for(auto child : children)
            child->subdivide(vertices, tris);

        divided = true;

        /*
        for (std::unique_ptr<OctreeNode> octr: children)
        {
            octr.subdivide(vertices);
        }*/

        return true;

    }
    else
    {
        //std::cout << "no es dividisble" << std::endl;
    }

    return false;
}

// Producto cruzado
std::array<float, 3> crossProduct(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

// Producto punto
float dotProduct(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Verifica la intersección entre una línea y un triángulo
bool edgeIntersectsTriangle(std::array<float, 3> p1, std::array<float, 3> p2, std::array<std::array<float, 3>, 3> triangle) {
    std::array<float, 3> u = {triangle[1][0] - triangle[0][0], triangle[1][1] - triangle[0][1], triangle[1][2] - triangle[0][2]};
    std::array<float, 3> v = {triangle[2][0] - triangle[0][0], triangle[2][1] - triangle[0][1], triangle[2][2] - triangle[0][2]};
    std::array<float, 3> n = crossProduct(u, v);

    std::array<float, 3> dir = {p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
    std::array<float, 3> w0 = {p1[0] - triangle[0][0], p1[1] - triangle[0][1], p1[2] - triangle[0][2]};
    float a = -dotProduct(n, w0);
    float b = dotProduct(n, dir);
    if (fabs(b) < 1e-6) {
        return false; // El segmento de línea es paralelo al triángulo
    }

    float r = a / b;
    if (r < 0.0 || r > 1.0) {
        return false; // No hay intersección
    }

    std::array<float, 3> intersection = {p1[0] + r * dir[0], p1[1] + r * dir[1], p1[2] + r * dir[2]};

    std::array<float, 3> uu = {u[0], u[1], u[2]};
    std::array<float, 3> uv = {v[0], v[1], v[2]};
    std::array<float, 3> wu = {intersection[0] - triangle[0][0], intersection[1] - triangle[0][1], intersection[2] - triangle[0][2]};
    std::array<float, 3> wv = {intersection[0] - triangle[2][0], intersection[1] - triangle[2][1], intersection[2] - triangle[2][2]};
    std::array<float, 3> nCrossU = crossProduct(n, uu);
    std::array<float, 3> nCrossV = crossProduct(n, uv);
    std::array<float, 3> nCrossW = crossProduct(n, wu);
    std::array<float, 3> nCrossWV = crossProduct(n, wv);

    float denom = dotProduct(nCrossU, nCrossV);
    float s = dotProduct(nCrossW, nCrossV) / denom;
    float t = dotProduct(nCrossU, nCrossWV) / denom;

    return (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0 && s + t <= 1.0);
}

// Verifica si algún borde del cubo se intersecta con el triángulo
bool checkIntersection(std::array<std::array<float, 3>, 3> triangle) {
    /*
    for (const auto& edge : cubeEdges) {
        if (edgeIntersectsTriangle(edge[0], edge[1], triangle)) {
            return true;
        }
    }
    return false;
    */
   return false;
}


// Calcula el código de región para un punto (x, y, z)
int OctreeNode::computeRegionCode(float x, float y, float z) {
    int code = INSIDE;

    if (x < boundary.min.x) code |= LEFT;
    else if (x > boundary.max.x) code |= RIGHT;
    if (y < boundary.min.y) code |= BOTTOM;
    else if (y > boundary.max.y) code |= TOP;
    if (z < boundary.min.z) code |= FRONT;
    else if (z > boundary.max.z) code |= BACK;

    return code;
}

// Función para verificar si un segmento de línea (p1, p2) se recorta contra el cubo
bool OctreeNode::cohenSutherlandClip(float x1, float y1, float z1, float x2, float y2, float z2) {
    
    int code1 = computeRegionCode(x1, y1, z1);
    int code2 = computeRegionCode(x2, y2, z2);

    bool accept = false;

    while (true) {
        if (!(code1 | code2)) {
            // Ambos puntos están dentro del cubo
            accept = true;
            break;
        } else if (code1 & code2) {
            // Ambos puntos comparten una región fuera del cubo
            break;
        } else {
            // Al menos un punto está fuera del cubo
            float x, y, z;

            // Elegir un punto fuera del cubo
            int codeOut = code1 ? code1 : code2;

            // Encuentra la intersección de este punto con el cubo
            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (boundary.max.y - y1) / (y2 - y1);
                y = boundary.max.y;
                z = z1 + (z2 - z1) * (boundary.max.y - y1) / (y2 - y1);
            } else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (boundary.min.y - y1) / (y2 - y1);
                y = boundary.min.y;
                z = z1 + (z2 - z1) * (boundary.min.y - y1) / (y2 - y1);
            } else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (boundary.max.x - x1) / (x2 - x1);
                x = boundary.max.x;
                z = z1 + (z2 - z1) * (boundary.max.x - x1) / (x2 - x1);
            } else if (codeOut & LEFT) {
                y = y1 + (y2 - y1) * (boundary.min.x - x1) / (x2 - x1);
                x = boundary.min.x;
                z = z1 + (z2 - z1) * (boundary.min.x - x1) / (x2 - x1);
            } else if (codeOut & BACK) {
                x = x1 + (x2 - x1) * (boundary.max.z - z1) / (z2 - z1);
                z = boundary.max.z;
                y = y1 + (y2 - y1) * (boundary.max.z - z1) / (z2 - z1);
            } else if (codeOut & FRONT) {
                x = x1 + (x2 - x1) * (boundary.min.z - z1) / (z2 - z1);
                z = boundary.min.z;
                y = y1 + (y2 - y1) * (boundary.min.z - z1) / (z2 - z2);
            }

            // Reemplaza el punto fuera del cubo con el punto de intersección
            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                z1 = z;
                code1 = computeRegionCode(x1, y1, z1);
            } else {
                x2 = x;
                y2 = y;
                z2 = z;
                code2 = computeRegionCode(x2, y2, z2);
            }
        }
    }
    return accept;
}

// Verifica si algún borde del triángulo se intersecta con el cubo
bool OctreeNode::checkTriangleIntersection(std::vector <Vertex> vertices, std::vector <Tri> tris) {


    for(auto tri: tris)
    {
        glm::vec3 v0 = vertices[tri.v0].position;
        glm::vec3 v1 = vertices[tri.v1].position;
        glm::vec3 v2 = vertices[tri.v2].position;


        if(cohenSutherlandClip(v0.x, v0.y, v0.z, v1.x, v1.y, v1.z))
        {
            return true;
        }
        if(cohenSutherlandClip(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z))
        {
            return true;
        }
        if(cohenSutherlandClip(v0.x, v0.y, v0.z, v2.x, v2.y, v2.z))
        {
            return true;
        }
    }

    return false;
}




bool OctreeNode::is_divisible(std::vector <Vertex> vertices, std::vector <Tri> tris)
{


    // Verificar si existe algun vertice dentro de la seccion 
    for(Vertex ver: vertices)
    {
        if (ver.position.x >= boundary.min.x && ver.position.x <= boundary.max.x &&
            ver.position.y >= boundary.min.y && ver.position.y <= boundary.max.y &&
            ver.position.z >= boundary.min.z && ver.position.z <= boundary.max.z)

            return true;
    }

    if (checkTriangleIntersection(vertices, tris))
    {
        return true;
    }


    return false;
}