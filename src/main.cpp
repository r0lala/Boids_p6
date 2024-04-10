// #include <cstdlib>
// #include "boids/boid.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <time.h>
#include <vector>
// #include "3D/default_shader.hpp"
#include "3D/vao.hpp"
#include "3D/vbo.hpp"
#include "boids/swarm.hpp"
#include "doctest/doctest.h"
// // #include "p6/p6.h"
// #include <glm/glm.hpp>
// #include "random/rand.hpp"

int main()
{
    // TODO : changer la direction pour qu'elle soit aléatoire
    Swarm groupe(50);
    srand(time(NULL)); // TODO à déplacer ?

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}}; // TODO Bee Boids
    ctx.maximize_window();

    // Param UI
    // TODO regrouper var et coeff ?
    // TODO regrouper dans un nouveau fichier ?
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

    // --- 3D ---
    VBO vbo;
    vbo.bind();

    // Fill buffer
    //  Creation tab of float containing coordinates of my points
    GLfloat vertices[] = {
        -0.5f, -0.5f, 1.f, 0.f, 0.f,
        0.5f, -0.5f, 0.f, 1.f, 0.f,
        0.0f, 0.5f, 0.f, 0.f, 1.f
    };

    // Sending the data
    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    vbo.unbind();

    // VAO
    VAO vao;
    vao.bind();

    // Activation vertex
    static constexpr GLuint aVertexPosition = 0;
    glEnableVertexAttribArray(aVertexPosition);
    static constexpr GLuint aVertexColor = 1;
    glEnableVertexAttribArray(aVertexColor);

    vbo.bind();
    glVertexAttribPointer(aVertexPosition, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(0 * sizeof(GLfloat)));
    glVertexAttribPointer(aVertexColor, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
    vbo.unbind();
    vao.unbind();

    // ---

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::VermilionPlochere);
        ctx.square(p6::Center{0., 0.}, p6::Radius{0.8f}, p6::Rotation{0.0_turn});

        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.05f}
        );

        groupe.draw(ctx);
        groupe.animate(ctx, align, separate, cohesion, coeffAlignement, coeffRepulsion, coeffCohesion, ctx.delta_time());

        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind VAO
        vao.bind();

        // Shader
        // glimac::bind_default_shader();
        // shader.use();

        // Draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Unbind vao
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}