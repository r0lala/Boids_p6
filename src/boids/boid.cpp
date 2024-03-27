#include "boid.hpp"
#include <cstdlib>
#include <iostream>
#include "glm/exponential.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
// #include "random/rand.hpp"
#include <cmath>
#include <limits>

bool Boid::operator==(const Boid& other) const
{
    return other._position == _position && other._velocity == _velocity;
}

glm::vec2 Boid::limitAcceleration(glm::vec2 acceleration, float maxAcceleration)
{
    if (glm::length(acceleration) > 0.001)
    {
        return glm::normalize(acceleration) * 0.001f;
    }
    return acceleration;
}

glm::vec2 Boid::limitAcceleration(glm::vec2 acceleration, float maxAcceleration)
{
    if (glm::length(acceleration) > 0.001)
    {
        return glm::normalize(acceleration) * 0.001f;
    }
    return acceleration;
}

void Boid::move(glm::vec2 acceleration, float delta_time)
{
    _velocity += acceleration * delta_time;
    _position += _velocity * delta_time;
}

glm::vec2 Boid::separation(const std::vector<Boid>& others, float zoneSeparation, float coeffSeparation)
{
    glm::vec2 direct(0, 0);
    int       count = 0;

    for (int i = 0; i < others.size(); i++)
    { // TODO boids => others
        double distance = glm::distance(this->_position, others[i]._position);

        if (distance > 0 && distance < zoneSeparation)
        {
            glm::vec2 diff(0., 0.);
            diff = _position - others[i]._position;
            glm::normalize(diff);
            diff /= distance; // Weight by distance
            direct += diff;
            count++;
        }
    }

    if (count > 0)
    {
        direct /= count;

        // direct[1] /= count;
    }
    if (std::sqrt(direct[0] * direct[0] + direct[1] * direct[1]) > 0)
    {
        glm::normalize(direct);
        // direct[0] *=
        direct -= _velocity;
    }
    direct = this->limitAcceleration(direct, 100.);
    return direct * coeffSeparation;
}

glm::vec2 Boid::cohesion(const std::vector<Boid>& boids, float zone, float coeff)
{
    // search the closest friend
    Boid  closestBoid = boids[0];
    float closest     = std::numeric_limits<float>::infinity();

    for (const Boid& myFriend : boids)
    {
        if (*this == myFriend)
            continue;

        float friendDistance = glm::distance(this->_position, myFriend._position);

        if (closest > friendDistance && friendDistance <= zone)
        {
            closest     = friendDistance;
            closestBoid = myFriend;
        }
    }

    // go to him
    if (closest != std::numeric_limits<float>::infinity())
    {
        glm::vec2 difference = glm::normalize(closestBoid.position() - this->position());
        return glm::normalize(this->_velocity - difference) * coeff;
    }
    return glm::vec2(0., 0.);
}

glm::vec2 Boid::alignement(const std::vector<Boid>& boids, float zoneAlignement, float coeffAlignement)
{
    glm::vec2 sum   = this->_position;
    float     count = 0;

    for (int i = 0; i < boids.size(); i++)
    {
        float d = glm::distance(this->_position, boids[i]._position);
        if ((d > 0) && (d < zoneAlignement))
        {
            sum += boids[i]._velocity;
            count++;
        }
    }
    if (count > 0)
    {
        // faire la moyenne
        sum = glm::normalize((sum / count) * coeffAlignement);
        sum = this->limitAcceleration(sum, 100.);
        return sum;
    }
    else
    {
        glm::vec2 lonely(0, 0);
        return lonely;
    }
}

// void Boid::collide(const Boid& other)
// {
//     if (std::abs(_position[0] - other._position[0]) < 2. * _size
//         && std::abs(_position[1] - other._position[1]) < 2. * _size)
//     {
//         // TODO rand

//         _velocity[0] = -_velocity[0]; // * (rand() % 2 + 0.1);
//         _velocity[1] = -_velocity[1]; //* (rand() % 2 + 0.1);
//     }
// }

// TODO change direction aléatoire

void Boid::bounceBorder(p6::Context& ctx) // TODO remove ctx param
{
    // constexpr float eps    = 0.02; // TODO comparaison de float
    constexpr float radius = 0.8; // TODO param ?

    if (radius - _size < _position[1])
    {
        _position[1] = radius - _size;
        _velocity *= -1.;
    }
    if (radius - _size < -_position[1])
    {
        _position[1] = -(radius - _size);
        _velocity *= -1.;
    }
    if (radius - _size < _position[0])
    {
        _position[0] = radius - _size;
        _velocity *= -1.;
    }
    if (-radius + _size > _position[0])
    {
        _position[0] = -radius + _size;
        _velocity *= -1.;
    }
}

void Boid::draw(p6::Context& ctx) const
{
    // TODO size
    ctx.circle(
        p6::Center{_position}, // center = _position de mon boid
        p6::Radius{_size}
    );
}
