#pragma once

#include <glm/glm.hpp>

namespace glimac {

struct ShapeVertex {
    // TODO rename fichier -> ShapeVertex
    // TODO add _attribut
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

} // namespace glimac

struct Vertex3D {
    glm::vec3 position;
    glm::vec2 texture;

    Vertex3D(){};

    Vertex3D(glm::vec3 pos, glm::vec2 col)
    {
        position = pos;
        texture  = col;
    }
};
