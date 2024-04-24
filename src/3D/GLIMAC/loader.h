#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "glm/glm.hpp"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust triangulation. Requires C++11
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};
struct Object3D {
    std::vector<vertex> vertices;
};

Object3D loadOBJ(
    const char* path
);
