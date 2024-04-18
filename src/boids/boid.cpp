#include "boid.hpp"
#include <cstdlib>
#include <iostream> // TODO debug
#include "glm/exponential.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
// #include "random/rand.hpp"
#include <cmath>
#include <limits>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"

bool Boid::operator==(const Boid& other) const
{
    return other._position == _position && other._velocity == _velocity;
}

glm::vec2 Boid::limitAcceleration(glm::vec2 acceleration, float maxAcceleration)
{
    if (glm::length(acceleration) > maxAcceleration)
    {
        return glm::normalize(acceleration) * maxAcceleration;
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
    direct = this->limitAcceleration(direct, 1.f);
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
        return this->limitAcceleration(glm::normalize(this->_velocity - difference) * coeff, 0.5f);
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
        sum = this->limitAcceleration(sum, 1.f);
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

void Boid::teleport()
{
    // constexpr float eps    = 0.02; // TODO comparaison de float
    constexpr float radius = 0.8; // TODO param ?
    // TODO besoin de size ?
    if (_position.x > radius)
    {
        _position.x = -radius;
    }
    if (_position.x < -radius)
    {
        _position.x = radius;
    }
    if (_position.y > radius)
    {
        _position.y = -radius;
    }
    if (_position.y < -radius)
    {
        _position.y = radius;
    }
}