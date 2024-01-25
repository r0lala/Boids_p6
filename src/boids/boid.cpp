#include "boid.hpp"
#include <cstdlib>
#include <iostream>

void Boid::move(p6::Context& ctx)
{
    m_coord += m_direction;
    comeBack(ctx);
}

void Boid::comeBack(p6::Context& ctx)
{
    constexpr float eps = 0.02;

    if (1 - m_coord[1] < eps)
    {
        m_direction[1] = -m_direction[1];
    }
    if (1 + m_coord[1] < -eps)
    {
        m_direction[1] = -m_direction[1];
    }
    if (ctx.aspect_ratio() - m_coord[0] < eps)
    {
        m_direction[0] = -m_direction[0];
    }
    if (-ctx.aspect_ratio() - m_coord[0] > eps)
    {
        m_direction[0] = -m_direction[0];
    }
}

void Boid::draw(p6::Context& ctx)
{
    ctx.circle(
        p6::Center{m_coord}, // center = m_coord de mon boid
        p6::Radius{0.05f}    // TODO attribut ?
    );
}