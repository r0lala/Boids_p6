#pragma once
#include "vao.hpp"
#include "shader.hpp"
#include "GLIMAC/common.hpp"
#include "p6/p6.h"

class Bush{
public:
    // TODO réduire le nb de param
    void draw(
        p6::Context &ctx,
        VAO &vao, Shader &tree,
        const std::vector<glimac::ShapeVertex>& vertices,
        const glm::mat4 &viewMatrix, GLuint textures
    );
};