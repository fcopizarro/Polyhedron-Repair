#pragma once    

#include "common.hpp"
#include <array> 

class OctreeNode
{
public: 

    BoundingBox boundary;

    int depth;
    int max_depth;

    //int capacity;
    
    bool divided;
    bool to_convert;

    OctreeNode *children[8];

    OctreeNode(const BoundingBox& boundary, int depth, int max_depth)
        : boundary(boundary), depth(depth), max_depth(max_depth), divided(false), to_convert(false) {}

    bool subdivide(const std::vector<std::shared_ptr<Vertex>>, std::vector <Tri>);

    int computeRegionCode(float x, float y, float z);
    bool cohenSutherlandClip(float x1, float y1, float z1, float x2, float y2, float z2);
    bool checkTriangleIntersection(std::vector<std::shared_ptr<Vertex>> vertices, std::vector <Tri> tris);


private:

    bool is_divisible(std::vector<std::shared_ptr<Vertex>>, std::vector <Tri>);
    
};

// Códigos de región
enum RegionCode {
    INSIDE = 0,  // 0000
    LEFT = 1,    // 0001
    RIGHT = 2,   // 0010
    BOTTOM = 4,  // 0100
    TOP = 8,     // 1000
    FRONT = 16,  // 10000
    BACK = 32    // 100000
};