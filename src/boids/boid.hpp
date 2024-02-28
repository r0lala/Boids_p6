#pragma once
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 _coord = {0, 0}; // m_z

    // bool  _neighbor;
    glm::vec2 _direction;
    float     _size = 0.05f;

public:
    Boid(const glm::vec2& coord, const glm::vec2& direction = p6::random::direction())
        : _coord(coord), _direction(direction) {}

    // Methods
    void move(p6::Context& ctx);
    // void bounceBorder(p6::Context& ctx); // TODO private ?
    void draw(p6::Context& ctx) const;
    // void collide(const Boid& other);

    bool operator==(const Boid& other) const;

private:
    void bounceBorder(p6::Context& ctx);

    //  //Behaviour
    // Cohesion

    // Separation

    // Alignment

public:
    float     distanceBetweenBoids(Boid stranger);
    glm::vec2 coord() const
    {
        return _coord;
    }
};
