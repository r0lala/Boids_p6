#pragma once
#include <iostream> // TODO test
#include <vector>
#include "3D/GLIMAC/common.hpp"
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 _position = {0, 0}; // m_z

    // bool  _neighbor;
    // bool _predator;
    glm::vec2 _velocity;
    // glm::vec2 _acceleration;
    float     _size = 0.09f;
    glm::vec2 seek(const glm::vec2& v);

public:
    Boid(const glm::vec2& position, const glm::vec2& velocity) // TODO velocity random
        : _position(position), _velocity(velocity)
    {}

    // Methods
    void move(glm::vec2 acceleration, float delta_time);
    void draw(
        p6::Context& ctx,
        VAO& vao, Shader& shader,
        const std::vector<glimac::ShapeVertex>& vertices,
        // TODO à supp par Bee
        Shader& wings, Shader& eyes, Shader& body, GLuint textures
    ) const;
    // void collide(const Boid& other);

    bool operator==(const Boid& other) const;

    void teleport();

public:
    float distanceBetweenBoids(const Boid& stranger) const;
    void  setvelocity(const glm::vec2& velocity) // TODO setVelocity
    {
        _velocity = velocity;
    }
    glm::vec2 limitAcceleration(glm::vec2 acceleration, float maxAcceleration);
    glm::vec2 velocity() const { return _velocity; }
    glm::vec2 position() const { return _position; }

    glm::vec2 separation(const std::vector<Boid>& boids, float zoneSeparation, float coeffSeparation);
    glm::vec2 cohesion(const std::vector<Boid>& boids, float zone, float coeff);
    glm::vec2 alignement(const std::vector<Boid>& boids, float zoneAlignement, float coeffAlignement);
};
