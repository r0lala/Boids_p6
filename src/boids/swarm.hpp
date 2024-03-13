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
    Swarm(int n = 5) // constructeur (par défaut?)
    {
        for (int i = 1; i <= n; i++)
        {
            // TODO réajuster "l'aléatoire"
            double    sign      = pileFace(0.5f) ? 1. : -1.;
            glm::vec2 direction = glm::vec2(0.001 * sign * ((double)(i % 5) + 0.001), 0.001 * ((double)(i) + 0.001));
            glm::vec2 coord     = glm::vec2(rand() % 5 / 10., rand() % 5 / 10.);
            _swarm.push_back(Boid(coord, direction));
        }
    }
    void draw(p6::Context& ctx) const;
    void animate(p6::Context& ctx); // faire bouger

private:
    // Méthodes
    // // Behavior
    void repulse(const Boid& pshit);              // zone de répulsion
    void follow(Boid& sheperd, p6::Context& ctx); // zone de follow
    void stick(Boid& glue);                       // zone d'attraction
    void bark();                                  // ouaf repulsion mouton sur le curseur (repulsion quand on clic)
};