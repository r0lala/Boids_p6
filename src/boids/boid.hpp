#pragma once
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 _coord; // m_z
    // bool  _neighbor;
    glm::vec2 _direction;
    float     _size = 0.05f;

public:
    Boid(const glm::vec2& coord, const glm::vec2& direction = p6::random::direction())
        : _coord(coord), _direction(direction) {}

    // Methods
    void move(p6::Context& ctx);
    void comeBack(p6::Context& ctx);
    void draw(p6::Context& ctx);
    void collide(Boid& other);

    //  //Behaviour
    // Cohesion

    // Separation

    // Alignment

    glm::vec2 coord() const
    {
        return _coord;
    }
};
