#pragma once

#include <vector>
#include "boids/boid.hpp"
#include "random/rand.hpp"

class Swarm {
private:
    std::vector<Boid> _swarm; // vecteur qui contient tous les boids

    // float _avoidRadius   = 60;   // rayon d'évitement
    // float _avoidStrength = 0.02; // puissance de répulsion

public:
    Swarm(int n = 5) // constructeur (par défaut?)
    {
        for (int i = 1; i <= n; i++)
        {
            // TODO réajuster "l'aléatoire"
            glm::vec2 direction = glm::vec2(0.001, 0.001);
            glm::vec2 coord     = glm::vec2(rand() % 5 / 10., rand() % 5 / 10.); // TODO faire une fonciton rand(min, max)
            _swarm.push_back(Boid(coord, direction));
        }
    }
    void draw(p6::Context& ctx) const;
    void animate(p6::Context& ctx, float zoneFollow, float zoneSeparate, float zoneCohesion, float coeffFollow, float coeffSeparate, float coeffCohesion, float delta_time); // faire bouger

private:
    // Méthodes
    // // Behavior
    void repulse(const Boid& boid, const float zoneRepulse, const float coeffRepulsion); // zone de répulsion
    void follow(const Boid& sheperd, const float zoneFollow, const float coeffFollow);   // zone de follow
    void stick(Boid& glue);                                                              // zone d'attraction
    void bark();                                                                         // ouaf repulsion mouton sur le curseur (repulsion quand on clic)
};