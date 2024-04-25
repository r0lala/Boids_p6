#pragma once
#include "GLIMAC/common.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include "shader.hpp"
#include "vao.hpp"

class Bush {
public:
    glm::vec3 _position; // TODO random // TODO private
    // TODO réduire le nb de param
    void draw(
        p6::Context& ctx,
        VAO& vao, Shader& tree,
        const std::vector<glimac::ShapeVertex>& vertices,
        const glm::mat4& viewMatrix, GLuint textures,
        glm::vec3 position, glm::vec3 scale, float degree
    );
};