#include "swarm.hpp"

// BOIDS ---
// this.friction  = 0.999;

// decelerate
// this.direction.mult(this.friction);

// ---
/*
void Swarm::repulse(Boid pshit)
{
    std::vector avoid;
    for (Boid stranger : _swarm)
    {
        // TODO distance entre stranger-pshit
        float personalSpace = pshit.distanceBetweenBoids(stranger);
        if (personalSpace < _avoidRad) // && personalSpace > 0
        {
            // float spray = pshit.coord() - stranger.coord();
            // TODO
        }
    }
}
*/
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