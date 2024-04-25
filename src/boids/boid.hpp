#pragma once
#include <vector>
#include "3D/GLIMAC/common.hpp"
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 _position = {0, 0, 0}; // m_z

    // bool  _neighbor;
    // bool _predator;
    glm::vec3 _velocity;
    // glm::vec2 _acceleration;

public:
    Boid(const glm::vec3& position, const glm::vec3& velocity) // TODO velocity random
        : _position(position), _velocity(velocity)
    {}

    // Methods
    bool operator==(const Boid& other) const;

    void move(glm::vec3 acceleration, float delta_time);
    void teleport();

public:
    float distanceBetweenBoids(const Boid& stranger) const;
    void  setvelocity(const glm::vec3& velocity) // TODO setVelocity
    {
        _velocity = velocity;
    }
    glm::vec3 limitAcceleration(glm::vec3 acceleration, float maxAcceleration);
    glm::vec3 velocity() const { return _velocity; }
    glm::vec3 position() const { return _position; }

    glm::vec3 separation(const std::vector<Boid>& boids, float zoneSeparation, float coeffSeparation);
    glm::vec3 cohesion(const std::vector<Boid>& boids, float zone, float coeff);
    glm::vec3 alignement(const std::vector<Boid>& boids, float zoneAlignement, float coeffAlignement);
};
