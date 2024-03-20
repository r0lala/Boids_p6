#include "boid.hpp"
#include <cstdlib>
#include <iostream>
#include "glm/exponential.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
// #include "random/rand.hpp"
#include <cmath>

bool Boid::operator==(const Boid& other) const
{
    return other._position == _position && other._velocity == _velocity;
}

// TODO changer pour que ce soit le swarm qui s'en occupe ?
void Boid::move(glm::vec2 acceleration)
{
    _velocity += acceleration;
    _position += _velocity;
}

glm::vec2 Boid::separation(const std::vector<Boid>& boids, float coeffSeparation)
{
    glm::vec2 direct(0, 0);
    int       count = 0;

    for (int i = 0; i < boids.size(); i++)
    { // TODO boids => others
        double distance = this->distanceBetweenBoids(boids[i]);

        if (distance > 0 && distance < coeffSeparation)
        {
            glm::vec2 diff(0., 0.);
            diff = _position - boids[i]._position;
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
    return direct;
}

glm::vec2 Boid::cohesion(const std::vector<Boid>& boids, float coeffCohesion)
{
    // float     neighbor = 20;
    glm::vec2 sum(0, 0);
    int       count = 0;
    for (int i = 0; i < boids.size(); i++)
    {
        float d = this->distanceBetweenBoids(boids[i]);

        if ((d > 0) && (d < coeffCohesion))
        {
            sum += boids[i]._velocity;
            count++;
        }
    }

    if (count > 0)
    {
        sum /= count;
        return seek(sum);
    }
    else
    {
        glm::vec2 lonely(0, 0);
        return lonely;
    }
}

glm::vec2 Boid::seek(const glm::vec2& v)
{
    glm::vec2 desired;
    ;
    desired -= v; // A vector pointing from the location to the target
    // Normalize desired and scale to maximum speed
    glm::normalize(desired);
    // desired *= maxSpeed;
    // Steering = Desired minus Velocity
    glm::vec2 acceleration = desired - _velocity;
    // acceleration.limit(maxForce); // Limit to maximum steering force
    return acceleration;
}

glm::vec2 Boid::alignement(const std::vector<Boid>& boids, float coeffAlignement)
{
    glm::vec2 sum(0, 0);
    int       count = 0;
    for (int i = 0; i < boids.size(); i++)
    {
        float d = this->distanceBetweenBoids(boids[i]);
        if ((d > 0) && (d < coeffAlignement))
        {
            sum += boids[i]._velocity;
            count++;
        }
    }
    if (count > 0)
    {
        sum /= count;
        glm::normalize(sum);
        // sum *= maxSpeed

        glm::vec2 steer;
        steer = sum - _velocity;
        // steer.limit(maxForce);
        return steer;
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
    constexpr float radius = 0.5; // TODO param ?

    if (radius - _size < _position[1])
    {
        _position[1] = radius - _size;
        _velocity[1] = -_velocity[1];
    }
    if (radius - _size < -_position[1])
    {
        _position[1] = -(radius - _size);
        _velocity[1] = -_velocity[1];
    }
    if (radius - _size < _position[0])
    {
        _position[0] = radius - _size;
        _velocity[0] = -_velocity[0];
    }
    if (-radius + _size > _position[0])
    {
        _position[0] = -radius + _size;
        _velocity[0] = -_velocity[0];
    }
}

void Boid::draw(p6::Context& ctx) const
{
    // TODO size
    constexpr float size = 0.05f;
    ctx.circle(
        p6::Center{_position}, // center = _position de mon boid
        p6::Radius{size}
    );
}

float Boid::distanceBetweenBoids(const Boid& stranger) const
{
    float distance = glm::sqrt(glm::pow(this->_position[0] - stranger._position[0], 2) + glm::pow(this->_position[1] - stranger._position[1], 2));

    return distance;
}