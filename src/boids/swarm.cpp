#include "swarm.hpp"
#include <vector>
#include "3D/bee.hpp" // TODO à supp
#include "boids/boid.hpp"
#include "glm/fwd.hpp"

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
    float deltatime
)
{
    for (Boid& b : _swarm)
    {
        glm::vec3 acceleration{0.f};

        // TODO à terminer ?
        acceleration += b.separation(_swarm, zoneSeparate, coeffSeparate);
        acceleration += b.cohesion(_swarm, zoneCohesion, coeffCohesion);
        acceleration += b.alignement(_swarm, zoneFollow, coeffFollow);
        acceleration = b.limitAcceleration(acceleration, 1.);
        b.setvelocity(b.limitAcceleration(b.velocity(), 1.));
        b.move(acceleration, deltatime);
        b.teleport();
    }
}