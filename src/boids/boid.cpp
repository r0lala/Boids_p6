#include "boid.hpp"
#include <iostream>

void Boid::move()
{
    m_coord += m_direction;
    comeBack();
}

void Boid::comeBack()
{
    constexpr float eps = 0.02;
    // if (m_coord[0] > ctx.aspect_ratio() || m_coord[0] < -ctx.aspect_ratio())

    // if (m_coord[0] - ctx.aspect_ratio() < 0.02)
    // {
    //     this->m_direction = -this->m_direction;
    // }
    // if (m_coord[0])

    if (1 - m_coord[1] < eps)
    {
        m_direction = -m_direction;
    }
    if (1 + m_coord[1] < -eps)
    {
        m_direction = -m_direction;
    }

    // si coor-bord< Epsilon alors
    // direction = -direction
}

void Boid::draw(p6::Context& ctx)
{
    ctx.circle(
        p6::Center{m_coord}, // center = m_coord de mon boid
        p6::Radius{0.05f}    // TODO attribut ?
    );
}