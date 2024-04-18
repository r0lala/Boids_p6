// TODO faire le tri des includes ...
#include <cstdlib>
#include "boids/boid.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/trigonometric.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <ctime>
#include <glm/glm.hpp>
#include <vector>
#include "3D/bee.hpp"
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
    // Boid  test = groupe[0]; // TODO test
    srand(time(NULL)); // TODO à déplacer ?
    Bee beez;

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Bee Boids"}};
    ctx.maximize_window();

    // Param UI
    Options options(ctx);

    // --- 3D ---

    // Creation Shader
    Shader body("3D", "bee/body");
    Shader eyes("3D", "bee/eyes");
    Shader wings("3D", "bee/wings");

    // Chargement des textures
    // TODO rename triforce
    // TODO bug : c'est chargé à l'envers ???
    img::Image triforce = p6::load_image_buffer("../assets/textures/bodyTexture.png");
    // std::unique_ptr<Image> triforce = loadImage("~/IMAC2/S4/GLImac-Template/assets/textures/triforce.png");
    // assert(triforce != NULL && "error loading triforce.png");

    VBO vbo;
    vbo.bind();

    // Fill buffer
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

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

    // beez.initBee(vbo, vao);
    // // Activation vertex
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
    GLuint textures;
    glGenTextures(1, &textures);
    glBindTexture(GL_TEXTURE_2D, textures);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        triforce.width(), triforce.height(),
        0, GL_RGBA, GL_UNSIGNED_BYTE, triforce.data()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Clear the window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ctx.background(p6::NamedColor::VermilionPlochere);

        // ctx.square(p6::Center{0., 0.}, p6::Radius{0.8f}, p6::Rotation{0.0_turn});

        // body.use();

        // // TODO mul mouse en fct de la taille de la sphere : 0.5f => size actuel
        // glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(ctx.mouse() * ctx.aspect_ratio() * (1.5f + 0.5f / 2.f), -5));
        // MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});

        // body.giveMatrix(ctx, MVMatrix);
        // vao.bind();
        // glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        // vao.unbind();

        groupe.draw(
            ctx, vao, vertices,
            wings, eyes, body, textures
        );

        groupe.animate(
            ctx,
            options.align,
            options.separate, options.cohesion,
            options.coeffAlignement, options.coeffRepulsion, options.coeffCohesion,
            ctx.delta_time()
        );

        // TODO adapter le nb de vertices en fonction de la taille qu'elle représente ?
        beez.draw(ctx, vao, vertices, wings, eyes, body, textures, glm::vec3(ctx.mouse() * ctx.aspect_ratio() * (1.5f + 0.5f / 2.f), -5));
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    // TODO dans un fichier
    glDeleteTextures(1, &textures);
    return EXIT_SUCCESS;
}