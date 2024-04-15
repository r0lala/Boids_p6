// #pragma once
#ifndef __BEE_HPP__
#define __BEE_HPP__
// #include <vector>
// #include "glm/glm.hpp"
#include "3D/glimac/common.hpp"
#include "p6/p6.h"
#include "shader.hpp"
#include "vao.hpp"

// #include "vbo.hpp"

class Bee {
    // Attributs
private:
    // Shader body("3D", "bee/body");
    // Shader eyes("3D", "bee/eyes");
    // Shader wings("3D", "bee/wings");

    // glm::vec3 _position = {0, 0, 0}; // m_z
    // glm::vec3 _velocity;
    //  glm::vec2 _acceleration;

public:
    // Bee(const glm::vec3& position, const glm::vec3& velocity) // TODO velocity random
    //     : _position(), _velocity(velocity)
    // {}
    Bee(){};

    // Methods
    // void move(glm::vec3 acceleration, float delta_time);
    // void initBee(VBO& vbo, VAO& vao);
    void drawWing(
        p6::Context& ctx, float angle,
        VAO& vao, glm::mat4& bodyMatrix, Shader& wings,
        const std::vector<glimac::ShapeVertex>& vertices
    );
    void drawFace(
        p6::Context& ctx, VAO& vao, Shader& eyes,
        const std::vector<glimac::ShapeVertex>& vertices
    );
    void drawBody(Shader& body, VAO& vao, p6::Context& ctx, const std::vector<glimac::ShapeVertex>& vertices, GLuint textures);

    // void drawBee(p6::Context& ctx) const;
};
#endif