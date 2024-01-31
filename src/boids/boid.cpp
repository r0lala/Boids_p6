#include "boid.hpp"
#include <cstdlib>
#include <iostream>

void Boid::move(p6::Context& ctx)
{
    _coord += _direction;
    comeBack(ctx);
}

void Boid::comeBack(p6::Context& ctx)
{
    constexpr float eps    = 0.02; // TODO comparaison de float
    constexpr float radius = 0.5;  // TODO param ?

    if (radius - _size < _coord[1])
    {
        _direction[1] = -_direction[1];
    }
    if (radius - _size < -_coord[1])
    {
        _direction[1] = -_direction[1];
    }
    if (radius - _size < _coord[0])
    {
        _direction[0] = -_direction[0];
    }
    if (-radius + _size > _coord[0])
    {
        _direction[0] = -_direction[0];
    }
}

void Boid::draw(p6::Context& ctx)
{
    ctx.circle(
        p6::Center{_coord}, // center = _coord de mon boid
        p6::Radius{_size}
    );
}