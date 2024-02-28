#include "boid.hpp"
#include <cstdlib>
#include <iostream>
#include "glm/exponential.hpp"
#include "glm/ext/quaternion_exponential.hpp"
// #include "random/rand.hpp"

bool Boid::operator==(const Boid& other) const
{
    return other._coord == _coord && other._direction == _direction;
}

void Boid::move(p6::Context& ctx)
{
    _coord += _direction;
    bounceBorder(ctx);
}

// void Boid::collide(const Boid& other)
// {
//     if (std::abs(_coord[0] - other._coord[0]) < 2. * _size
//         && std::abs(_coord[1] - other._coord[1]) < 2. * _size)
//     {
//         // TODO rand

//         _direction[0] = -_direction[0]; // * (rand() % 2 + 0.1);
//         _direction[1] = -_direction[1]; //* (rand() % 2 + 0.1);
//     }
// }

// TODO change direction aléatoire

void Boid::bounceBorder(p6::Context& ctx)
{
    // constexpr float eps    = 0.02; // TODO comparaison de float
    constexpr float radius = 0.5; // TODO param ?

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

void Boid::draw(p6::Context& ctx) const
{
    // TODO size
    constexpr float size = 0.05f;
    ctx.circle(
        p6::Center{_coord}, // center = _coord de mon boid
        p6::Radius{size}
    );
}

float Boid::distanceBetweenBoids(Boid stranger)
{
    float distance = glm::sqrt(glm::pow(this->_coord[0] - stranger._coord[0], 2) + glm::pow(this->_coord[1] - stranger._coord[1], 2));

    return distance;
}