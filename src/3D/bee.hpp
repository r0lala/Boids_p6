// #pragma once
#ifndef __BEE_HPP__
#define __BEE_HPP__
// #include <vector>
// #include "glm/glm.hpp"
#include "3D/glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "p6/p6.h"
#include "shader.hpp"
#include "vao.hpp"


// #include "vbo.hpp"

class Bee {
    // Attributs
    // TODO faire tri private / public
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

private:
    // Methods
    // void move(glm::vec3 acceleration, float delta_time);
    // void initBee(VBO& vbo, VAO& vao);
    glm::mat4 giveWing(float angle);
    glm::mat4 giveFace(float angle);
    glm::mat4 giveBody();

private:
    // TODO à mettre dans une autre classe ?
    void render(
        VAO&                                    vao,
        const std::vector<glimac::ShapeVertex>& vertices,
        Shader& body, GLuint textures = -1, int textUnit = -1
    );

public:
    void draw(
        p6::Context& ctx, VAO& vao,
        const std::vector<glimac::ShapeVertex>& vertices,
        Shader& wings, Shader& eyes, Shader& body, GLuint textures,
        glm::vec3 position, glm::vec3 scale,
        glm::mat4 ViewMatrix, float angle, glm::vec3 axe
    );
};
#endif