#pragma once
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 m_coord; // m_z
    // bool  m_neighbor;
    glm::vec2 m_direction;

public:
    Boid(glm::vec2 coord, glm::vec2 direction = p6::random::direction())
        : m_coord(coord), m_direction(direction) {}

    // Methods
    void move();
    void comeBack();
    void draw(p6::Context& ctx);

    //  //Behaviour
    // Cohesion

    // Separation

    // Alignment

    glm::vec2 coord() const
    {
        return m_coord;
    }
};
