#pragma once

#include <vector>
#include "boids/boid.hpp"
#include "glm/fwd.hpp"
#include "random/rand.hpp"

class Swarm {
private:
    std::vector<Boid> _swarm; // vecteur qui contient tous les boids
    float             _size;
    // float _avoidRadius   = 60;   // rayon d'évitement
    // float _avoidStrength = 0.02; // puissance de répulsion

public:
    Swarm(int n = 5, float size = 0.5f) // constructeur (par défaut?)
        : _size(size)
    {
        for (int i = 1; i <= n; i++)
        {
            // TODO réajuster "l'aléatoire"
            glm::vec3 direction = glm::vec3(rand() % 5 / 10. - 0.25, rand() % 5 / 10. - 0.25, 0.);
            glm::vec3 coord     = glm::vec3(rand() % 5 / 10., rand() % 5 / 10., -5.); // TODO faire une fonciton rand(min, max)
            _swarm.push_back(Boid(coord, direction));
        }
    }
    void draw(
        p6::Context&                            ctx,
        VAO&                                    vao,
        const std::vector<glimac::ShapeVertex>& vertices,
        Shader& wings, Shader& eyes, Shader& body, GLuint textures,
        glm::mat4 ViewMatrix
    ) const;
    void animate(p6::Context& ctx, float zoneFollow, float zoneSeparate, float zoneCohesion, float coeffFollow, float coeffSeparate, float coeffCohesion, float delta_time); // faire bouger

private:
    // Méthodes
    // // Behavior
    void repulse(const Boid& boid, const float zoneRepulse, const float coeffRepulsion); // zone de répulsion
    void follow(const Boid& sheperd, const float zoneFollow, const float coeffFollow);   // zone de follow
    void stick(Boid& glue);                                                              // zone d'attraction
    void bark();                                                                         // ouaf repulsion mouton sur le curseur (repulsion quand on clic)
};