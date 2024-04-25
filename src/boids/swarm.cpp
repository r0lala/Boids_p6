#include "swarm.hpp"
#include <vector>
#include "3D/bee.hpp" // TODO à supp
#include "boids/boid.hpp"
#include "glm/fwd.hpp"

// this.friction  = 0.999;

// decelerate
// this.direction.mult(this.friction);

// ---

// TODO fct voisin, réduire de repulse, follow, ...

// void Swarm::repulse(const Boid& boid, const float zoneRepulse, const float coeffRepulsion)
// {
//     std::vector<int> indexNeighbor;

//     for (int i = 0; i < _swarm.size(); i++)
//     {
//         Boid stranger = _swarm[i];
//         if (stranger.position().x == _swarm[i].position().x && stranger.position().y == _swarm[i].position().y)
//             continue;

//         float personalSpace = boid.distanceBetweenBoids(stranger);
//         if (personalSpace < zoneRepulse && personalSpace > 0.05)
//         {
//             indexNeighbor.push_back(i);
//         }
//     }

//     glm::vec2 spray(0., 0.);
//     for (int indexStranger : indexNeighbor)
//     {
//         spray += boid.position() - _swarm[indexStranger].position();
//         // if (spray.x > 0. && spray.y > 0.)
//         //     stranger.
//     }

//     spray /= indexNeighbor.size();
//     for (int indexStranger : indexNeighbor)
//     {
//         _swarm[indexStranger].setvelocity(glm::normalize(-spray) * coeffRepulsion);
//     }
// }

// void Swarm::follow(const Boid& sheperd, const float zoneFollow, const float coeffFollow)
// {
//     for (Boid& sheep : _swarm)
//     {
//         if (sheperd.distanceBetweenBoids(sheep) < zoneFollow)
//         {
//             glm::vec2 change = sheperd.position() - sheep.position();
//             if (change.x != 0. || change.y != 0.)
//                 sheep.setvelocity(glm::normalize(change) * coeffFollow); // 0.001
//         }
//     }
// }

// TODO faire un template
void Swarm::draw(
    p6::Context&                            ctx,
    VAO&                                    vao,
    const std::vector<glimac::ShapeVertex>& vertices,
    Shader& wings, Shader& eyes, Shader& body, GLuint textures,
    glm::mat4 ViewMatrix
) const
{
    Bee boid;

    for (const Boid& b : _swarm)
    {
        float     angle = glm::angle(glm::vec3{-1, 0, 0}, glm::normalize(b.velocity()));
        glm::vec3 axe   = glm::cross(glm::vec3{-1, 0, 0}, glm::normalize(b.velocity()));
        boid.draw(
            ctx, vao, vertices, wings, eyes, body, textures,
            b.position() * ctx.aspect_ratio() * (1.f + _size),
            glm::vec3(_size), ViewMatrix, angle, axe
        );
    }
}

void Swarm::animate(
    p6::Context& ctx, // TODO à supp ?
    float zoneFollow, float zoneSeparate, float zoneCohesion,
    float coeffFollow, float coeffSeparate, float coeffCohesion,
    float deltatime // TODO déjà inclus dans le ctx
)
{
    for (Boid& b : _swarm)
    {
        glm::vec3 acceleration{0.f};
        // this->follow(b, zoneFollow, coeffFollow);
        // this->repulse(b, zoneSeparate, coeffSeparate);

        // TODO à terminer
        // acceleration += b.separation(_swarm, zoneSeparate, coeffSeparate);
        acceleration += b.cohesion(_swarm, zoneCohesion, coeffCohesion);
        acceleration += b.alignement(_swarm, zoneFollow, coeffFollow);
        acceleration = b.limitAcceleration(acceleration, 1.);
        b.setvelocity(b.limitAcceleration(b.velocity(), 1.));
        b.move(acceleration, deltatime);
        b.teleport();
    }
}