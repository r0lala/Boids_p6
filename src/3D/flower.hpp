#pragma once
#include "3D/glimac/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "p6/p6.h"
#include "shader.hpp"
#include "vao.hpp"

class Flower {
private:
    glm::vec3 _position{0, 0, 0};

    Flower();

    void      moveFlower(p6::Context ctx, float wall);
    void      moveFront(float delta, float wall);
    void      moveBack(float delta, float wall);
    void      moveLeft(float delta, float wall);
    void      moveRight(float delta, float wall);
    glm::mat4 getViewMatrix() const;
};