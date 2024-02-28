#pragma once

#include <vector>
#include "boids/boid.hpp"
#include "random/rand.hpp"

class Swarm {
private:
    std::vector<Boid> _swarm; // vecteur qui contient tous les boids

    float _avoidRad = 60;   // rayon d'évitement
    float _avoidStr = 0.02; // puissance de répulsion

public:
    Swarm(int n = 5)
    {
        for (int i = 0; i < n; i++)
        {
            double    sign      = pileFace(0.5f) ? 1. : -1.;
            glm::vec2 direction = glm::vec2(0.001 * sign * ((double)(i % 5) + 0.001), 0.001 * ((double)(i) + 0.001));
            glm::vec2 coord     = glm::vec2(rand() % 5 / 10., rand() % 5 / 10.);
            _swarm.push_back(Boid(coord, direction));
        }
    }

private:
    // Méthodes
    void repulse(Boid pshit); // zone de répulsion
    void follow(Boid sheep);  // zone de follow
    void stick(Boid glue);    // zone d'attraction
    void animate();           // faire bouger
    void bark();              // ouaf repulsion mouton sur le curseur
};