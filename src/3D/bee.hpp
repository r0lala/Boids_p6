#pragma once
// #include <vector>
// #include "glm/glm.hpp"
// #include "p6/p6.h"
// #include "shader.hpp"
// #include "vao.hpp"
// #include "vbo.hpp"

class Bee {
    // Attributs
private:
    // glm::vec3 _position = {0, 0, 0}; // m_z
    // glm::vec3 _velocity;
    //  glm::vec2 _acceleration;

public:
    // Bee(const glm::vec3& position, const glm::vec3& velocity) // TODO velocity random
    //     : _position(), _velocity(velocity)
    // {}
    Bee();

    // Methods
    void move(glm::vec3 acceleration, float delta_time);
    void initBee(VBO& vbo, VAO& vao);
    // void drawWing(p6::Context ctx, float angle, VAO& vao, glm::mat4 bodyMatrix, Shader wings, std::vector<ShapeVertex> vertices) const;
    void drawFace();
    void drawBody();
    void drawBee(p6::Context& ctx) const;
};