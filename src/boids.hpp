#pragma once

class Boid {
private:
    Point m_x, m_y; // m_z
    // bool  m_neighbor;

public:
    // TODO template ?
    Boid(float x, float y)
        : m_x(x), m_y(y) {}
}