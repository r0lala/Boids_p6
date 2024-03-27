#include <cstdlib>
#include "boids/boid.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <time.h>
#include <vector>
#include "boids/swarm.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "random/rand.hpp"

int main()
{
    // TODO : changer la direction pour qu'elle soit aléatoire
    // Boid comparaison{glm::vec2(0, 0), glm::vec2(genDirection(), genDirection())};

    Swarm groupe(50);
    // int               pos = (int)rand01();

    srand(time(NULL));

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();
    auto align    = 0.3f;
    auto separate = 0.1f;
    auto cohesion = 0.6f;

    auto coeffAlignement = 0.001f;
    auto coeffRepulsion  = 1.f;
    auto coeffCohesion   = 1.f;

    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        // ImGui::SliderFloat("Alignment", &align, 0.f, 1.f);
        // ImGui::SliderFloat("Separation", &separate, 1.f, 2.f);
        ImGui::SliderFloat("Cohesion", &cohesion, 0.f, 100.f);
        ImGui::SliderFloat("Coefficient d'alignement", &coeffAlignement, 0.f, 50.f);
        ImGui::SliderFloat("Coefficient de repulsion", &coeffRepulsion, 0.f, 50.f);
        ImGui::SliderFloat("Coefficient de cohesion", &coeffCohesion, 0.f, 50.f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::VermilionPlochere);
        ctx.square(p6::Center{0., 0.}, p6::Radius{0.5f}, p6::Rotation{0.0_turn});

        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.05f}
        );

        groupe.draw(ctx);
        groupe.animate(ctx, align, separate, cohesion, coeffAlignement, coeffRepulsion, coeffCohesion, ctx.delta_time());

        // comparaison.draw(ctx);
        // comparaison.move();
        // comparaison.bounceBorder(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}