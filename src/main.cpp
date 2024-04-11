// TODO faire le tri des includes ...
#include <cstdlib>
#include "boids/boid.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <ctime>
#include <glm/glm.hpp>
#include <vector>
#include "3D/glimac/common.hpp"
#include "3D/glimac/default_shader.hpp"
#include "3D/glimac/sphere_vertices.hpp"
#include "3D/vao.hpp"
#include "3D/vbo.hpp"
#include "boids/swarm.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "random/rand.hpp"

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
    // TODO regrouper UI dans un nouveau fichier ?
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

    // Creation Shader
    // TODO verif path
    const p6::Shader shader = p6::load_shader(
        "../src/3D/shaders/3D.vs.glsl",
        "../src/3D/shaders/normals.fs.glsl"
    );

    VBO vbo;
    vbo.bind();

    // Fill buffer
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // Sending the data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(vertices),
        vertices.data(),
        GL_STATIC_DRAW
    );

    vbo.unbind();

    // VAO
    VAO vao;
    vao.bind();

    // Activation vertex
    vbo.bind();
    static constexpr GLuint aVertexPosition = 0;
    glEnableVertexAttribArray(aVertexPosition);
    glVertexAttribPointer(
        aVertexPosition, 3, GL_FLOAT, GL_FALSE,
        sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position))
    );

    static constexpr GLuint aVertexNormal = 1;
    glEnableVertexAttribArray(aVertexNormal);
    glVertexAttribPointer(
        aVertexNormal, 3, GL_FLOAT, GL_FALSE,
        sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal))
    );

    static constexpr GLuint aVertexTexCoords = 2;
    glEnableVertexAttribArray(aVertexTexCoords);
    glVertexAttribPointer(
        aVertexTexCoords, 2, GL_FLOAT, GL_FALSE,
        sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords))
    );
    vbo.unbind();
    vao.unbind();

    // ---

    glEnable(GL_DEPTH_TEST); // active le test de profondeur du GPU

    // Variables uniformes
    GLint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::VermilionPlochere);
        // ctx.square(p6::Center{0., 0.}, p6::Radius{0.8f}, p6::Rotation{0.0_turn});

        // ctx.circle(
        //     p6::Center{ctx.mouse()},
        //     p6::Radius{0.05f}
        // );

        // groupe.draw(ctx);
        // groupe.animate(ctx, align, separate, cohesion, coeffAlignement, coeffRepulsion, coeffCohesion, ctx.delta_time());

        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Shader
        // TODO glimac::bind_default_shader();
        shader.use();

        // Bind VAO
        vao.bind();

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        // Draw triangle
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // Unbind vao
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}