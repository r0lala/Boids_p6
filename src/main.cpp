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
// #include "3D/GLIMAC/camera.hpp"
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
    // Camera camera;

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}}; // TODO Bee Boids
    ctx.maximize_window();

    // ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
    //     // TODO
    //     std::cout << "dy = " << scroll.dy << std::endl;
    //     std::cout << "dx = " << scroll.dx << std::endl;

    //     camera.moveFront(scroll.dy / 10.);
    // };

    // ctx.mouse_dragged = [&](p6::MouseDrag mouse) {
    //     float y = mouse.delta[0];
    //     float x = mouse.delta[1];
    //     camera.rotateLeft(x);
    //     camera.rotateUp(y);
    // };

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
        "../src/3D/shaders/wall.vs.glsl",
        "../src/3D/shaders/wall.fs.glsl"
    );

    img::Image sky = p6::load_image_buffer(
        "../assets/textures/sky.jpg"
    );

    GLuint textureWall;
    glGenTextures(1, &textureWall);
    glBindTexture(GL_TEXTURE_2D, textureWall);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky.width(), sky.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sky.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    VBO vbo;
    vbo.bind();

    // Fill buffer
    // const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    Vertex3D vertices[] = {
        Vertex3D{{0.5f, -0.5f, -7}, {1.f, 0.f}},
        Vertex3D{{-0.5f, 0.5f, -7}, {1.f, 0.f}},
        Vertex3D{{-0.5f, -0.5f, -7}, {0.5f, 0.5f}},
        Vertex3D{{0.5f, -0.5f, -7}, {1.f, 0.f}},
        Vertex3D{{0.5f, 0.5f, -7}, {0.f, 1.f}},
        Vertex3D{{-0.5f, 0.5f, -7}, {0.f, 1.f}}
    };

    // Sending the data
    glBufferData(
        GL_ARRAY_BUFFER,
        6 * sizeof(Vertex3D),
        vertices,
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
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, position))
    );

    // static constexpr GLuint aVertexNormal = 1;
    // glEnableVertexAttribArray(aVertexNormal);
    // glVertexAttribPointer(
    //     aVertexNormal, 3, GL_FLOAT, GL_FALSE,
    //     sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal))
    // );

    static constexpr GLuint aVertexTexCoords = 2;
    glEnableVertexAttribArray(aVertexTexCoords);
    glVertexAttribPointer(
        aVertexTexCoords, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, texture))
    );
    vbo.unbind();
    vao.unbind();

    // ---

    glEnable(GL_DEPTH_TEST); // active le test de profondeur du GPU

    // Variables uniformes
    // GLint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    // GLint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    // GLint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

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
        GLuint uTexture = glGetUniformLocation(shader.id(), "uTexture");
        glBindTexture(GL_TEXTURE_2D, textureWall);
        glUniform1i(uTexture, 0);

        // Bind VAO
        vao.bind();

        // Draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Unbind vao
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}