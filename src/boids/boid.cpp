#include "boid.hpp"
#include <cstdlib>
#include <iostream>

void Boid::move(p6::Context& ctx)
{
    _coord += _direction;
    comeBack(ctx);
}

void Boid::collide(Boid& other)
{
    if (std::abs(_coord[0] - other._coord[0]) < 2. * _size
        && std::abs(_coord[1] - other._coord[1]) < 2. * _size)
    {
        _direction[1]       = -_direction[1];
        other._direction[1] = -other._direction[1];
    }
}

// TODO change direction aléatoire

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