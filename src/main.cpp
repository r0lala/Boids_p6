#include <cstdlib>
#include "boids/boid.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <time.h>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "random/rand.hpp"

int main()
{
    // TODO : changer la direction pour qu'elle soit aléatoire
    Boid              comparaison{glm::vec2(0, 0), glm::vec2(0.001, 0.001)};
    std::vector<Boid> boids;
    // int               pos = (int)rand01();

    srand(time(NULL));
    for (int i = 0; i < 7; i++)
    {
        double    sign      = pileFace(0.5f) ? 1. : -1.;
        glm::vec2 direction = glm::vec2(0.001 * sign * ((double)(i % 5) + 0.001), 0.001 * ((double)(i) + 0.001));
        boids.push_back(Boid(glm::vec2(rand() % 5 / 10., rand() % 5 / 10.), direction));
    }

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // ctx.background(p6::NamedColor::Blue);
        ctx.square(p6::Center{0., 0.}, p6::Radius{0.5f}, p6::Rotation{0.0_turn});

        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.05f}
        );
        // ctx.circle(
        //     p6::Center{boid.coord()}, // center = m_coord de mon boid
        //     p6::Radius{0.05f}
        // );

        for (Boid& b : boids)
        {
            comparaison.collide(b); // TODO for tests purpose
            b.draw(ctx);
            b.move(ctx);

            for (Boid& neighbor : boids)
            {
                if (b == neighbor)
                    continue;
                b.collide(neighbor);
            }
        }
        comparaison.draw(ctx);
        comparaison.move(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}