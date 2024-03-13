#include "swarm.hpp"
#include <iostream> // TODO pour les tests
#include "glm/fwd.hpp"
// BOIDS ---
// this.friction  = 0.999;

// decelerate
// this.direction.mult(this.friction);

// ---

void Swarm::repulse(const Boid& pshit)
{
    std::vector<Boid> avoid;
    for (Boid& stranger : _swarm)
    {
        // TODO distance entre stranger-pshit
        float personalSpace = pshit.distanceBetweenBoids(stranger);
        if (personalSpace < _avoidRad) // && personalSpace > 0
        {
            glm::vec2 spray = pshit.coord() - stranger.coord();

            stranger.setDirection(glm::normalize(spray) * _avoidStr);
        }
    }
}

void Swarm::follow(Boid& sheperd, p6::Context& ctx)
{
    for (Boid& sheep : _swarm)
    {
        float zoneFollow = 0.5;
        if (sheperd.distanceBetweenBoids(sheep) < zoneFollow)
        {
            // std::cout << "follow" << std::endl;
            glm::vec2 change = sheperd.coord() - sheep.coord();
            if (change.x != 0. || change.y != 0.)
                sheep.setDirection(glm::normalize(change) * 0.001f);
        }
    }
    // std::cout << "stop" << std::endl;
}

void Swarm::draw(p6::Context& ctx) const
{
    for (const Boid& b : _swarm)
    {
        b.draw(ctx);
    }
}

void Swarm::animate(p6::Context& ctx)
{
    for (Boid& b : _swarm)
    {
        b.move(ctx);
        this->repulse(b);
        this->follow(b, ctx);
    }
}

/*
function Swarm(swarmCount)
{
    // avoidance - don't get too close to your neighbours
    this.avoid = function(agent)
    {
        // vector to store avoidance force
        var avoidVec = createVector();

        // run through the swarm
        for (var neighbour of this.swarm)
        {
            // neighbour distance
            var nd = agent.pos.dist(neighbour.pos);

            // ignore neighbours that are far away
            if (nd < avoidRadius && nd > 0)
            {
                // repulsive push away from close neighbours
                var pushVec = p5.Vector.sub(agent.pos, neighbour.pos);

                // scale to 1
                pushVec.normalize();

                // add this push to the total avoidance
                avoidVec.add(pushVec);
            }
        }
        // scale to 1.0
        avoidVec.normalize();
        // multiply by the strength variable
        avoidVec.mult(avoidStrength);
        // add to velocity
        agent.vel.add(avoidVec);
    }
}
*/