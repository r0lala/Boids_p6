#include <cstdlib>
#include "boids/boid.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"


int main()
{
    Boid boid{glm::vec2(0, 0), glm::vec2(0.001, 0.001)};

    Boid boi{glm::vec2(0.1, 0.2), glm::vec2(0.005, 0.001)};

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
        boid.draw(ctx);
        boid.move(ctx);

        boi.draw(ctx);
        boi.move(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}