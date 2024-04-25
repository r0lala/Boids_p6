// TODO faire le tri des includes ...
#include <cstdlib>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <ctime>
#include <glm/glm.hpp>
#include <vector>
#include "3D/GLIMAC/camera.hpp"
#include "3D/bee.hpp"
#include "3D/bush.hpp" // TODO test
#include "3D/glimac/common.hpp"
#include "3D/glimac/sphere_vertices.hpp"
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "3D/vbo.hpp"
#include "boids/swarm.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "param/options.hpp"
#include "random/rand.hpp"

int main()
{
    // TODO : changer la direction pour qu'elle soit aléatoire
    Swarm groupe(50);
    srand(time(NULL)); // TODO à déplacer ?

    Camera camera;

    // TODO test, à supp plus tard
    Bee  beez;
    Bush bush;

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Bee Boids"}};
    ctx.maximize_window();

    // TODO => option.cpp ?
    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        camera.moveFront(scroll.dy / 10.);
    };

    ctx.mouse_dragged = [&](p6::MouseDrag mouse) {
        camera.rotateLeft(mouse.delta[1]); // x
        camera.rotateUp(mouse.delta[0]);   // y
    };

    // Param UI
    Options options(ctx);

    // --- 3D ---

    // Creation Shader
    Shader body("3D", "bee/body");
    Shader eyes("3D", "bee/eyes");
    Shader wings("3D", "normals");
    Shader tree("3D", "tree/leaf"); // TODO renamme bush ???
    // Shader grass("3D", "grass");

    // Chargement des textures
    img::Image beeBody = p6::load_image_buffer("../assets/textures/bodyTexture.png", false);
    img::Image leaf    = p6::load_image_buffer("../assets/textures/leaf.png", false);
    img::Image wall    = p6::load_image_buffer("../assets/textures/clouds.png", false);

    VBO vbo;
    vbo.bind();

    // Fill buffer
    std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);
    // Sending the data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glimac::ShapeVertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    vbo.unbind();

    // VAO
    VAO vao;
    vao.bind();

    // beez.initBee(vbo, vao); // TODO
    // // Activation vertex // TODO fct
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

    // Texture
    // TODO dans un fichier
    GLuint treeTexture;
    glGenTextures(1, &treeTexture);
    glBindTexture(GL_TEXTURE_2D, treeTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        leaf.width(), leaf.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, leaf.data()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint beeTexture;
    glGenTextures(1, &beeTexture);
    glBindTexture(GL_TEXTURE_2D, beeTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        beeBody.width(), beeBody.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, beeBody.data()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint wallTexture;
    glGenTextures(1, &wallTexture);
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        wall.width(), wall.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, wall.data()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ctx.background(p6::NamedColor::CeladonBlue);

        // TODO cube
        // ctx.square(p6::Center{0., 0.}, p6::Radius{0.8f}, p6::Rotation{0.0_turn});
        body.use();
        body.bindTexture(beeTexture);

        // TODO adapter le nb de vertices en fonction de la taille qu'elle représente ?
        beez.draw(
            ctx, vao, vertices,
            wings, eyes, body, beeTexture,
            glm::vec3(ctx.mouse() * ctx.aspect_ratio() * (1.5f + 0.5f / 2.f), -5.),
            glm::vec3(0.3), camera.getViewMatrix(), 0., glm::vec3{0, 1, 0}
        );

        groupe.draw(
            ctx, vao, vertices,
            wings, eyes, body, beeTexture,
            camera.getViewMatrix()
        );

        vbo.bind();
        switch (options.getLods())
        {
        case 1:
            vertices = glimac::sphere_vertices(1.f, 32, 16);
            break;
        case 0:
            vertices = glimac::sphere_vertices(1.f, 10, 4);
            break;

        default:
            vertices = glimac::sphere_vertices(1.f, 32, 16);
        };

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(glimac::ShapeVertex),
            vertices.data(),
            GL_STATIC_DRAW
        );
        vbo.unbind();
        tree.use();
        tree.bindTexture(treeTexture);

        std::vector<glm::vec3> scale = {
            glm::vec3({2.f, -5.f, 2.f}),
            glm::vec3({4.f, -5.f, -5.f}),
            glm::vec3({-1.f, -5.f, 0.f}),
            glm::vec3({7.f, -5.f, 0.4f}),
            glm::vec3({-6.f, -5.f, -4.f}),
            glm::vec3({0.2f, -5.f, -6.f}),
            glm::vec3({-1.f, -5.f, -7.f}),
            glm::vec3({-4.f, -5.f, 4.f})
        };

        for (unsigned int i = 0; i < scale.size(); i++)
        {
            bush.draw(ctx, vao, tree, vertices, camera.getViewMatrix(), treeTexture, scale[i], glm::vec3(3.), i * 8);
        }

        groupe.animate(
            ctx,
            options.align,
            options.separate, options.cohesion,
            options.coeffAlignement, options.coeffRepulsion, options.coeffCohesion,
            ctx.delta_time()
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    // TODO dans un fichier
    glDeleteTextures(1, &treeTexture);
    glDeleteTextures(1, &beeTexture);
    return EXIT_SUCCESS;
}