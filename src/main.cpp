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
#include "3D/GLIMAC/camera.hpp"
#include "3D/bee.hpp"
#include "3D/glimac/common.hpp"
#include "3D/glimac/default_shader.hpp"
#include "3D/glimac/sphere_vertices.hpp"
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
    Bee    beez;

    // Run the tests
    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    // Actual application code
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}}; // TODO Bee Boids
    ctx.maximize_window();

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
    Shader shader("3D", "wall");

    img::Image sky = p6::load_image_buffer(
        "../assets/textures/clouds.png"
    );

    GLuint textureWall;
    glGenTextures(1, &textureWall);
    glBindTexture(GL_TEXTURE_2D, textureWall);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky.width(), sky.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sky.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    Shader body("3D", "bee/body");
    Shader eyes("3D", "bee/eyes");
    Shader wings("3D", "normals");

    // Chargement des textures
    // TODO rename triforce
    img::Image triforce = p6::load_image_buffer("../assets/textures/bodyTexture.png", false);

    VBO vbo;
    vbo.bind();

    // Fill buffer
    const std::vector<glimac::ShapeVertex> verticesSphere = glimac::sphere_vertices(1.f, 32, 16);
    constexpr float                        fond           = -10.f;
    constexpr float                        bord           = 4.f;

    // TODO 4 = hauteur pour nos abeilles

    std::vector<Vertex3D> verticesWall = {
        Vertex3D{{bord, -bord, fond}, {1.f, 0.f}},
        Vertex3D{{-bord, bord, fond}, {1.f, 0.f}},
        Vertex3D{{-bord, -bord, fond}, {0.5f, 0.5f}},
        Vertex3D{{bord, -bord, fond}, {1.f, 0.f}},
        Vertex3D{{bord, bord, fond}, {0.f, 1.f}},
        Vertex3D{{-bord, bord, fond}, {0.f, 1.f}}
    };

    // Sending the data
    glBufferData(
        GL_ARRAY_BUFFER,
        verticesSphere.size() * sizeof(glimac::ShapeVertex),
        verticesSphere.data(),
        GL_STATIC_DRAW
    );

    vbo.unbind();

    VBO vboWall;
    vboWall.bind();

    glBufferData(
        GL_ARRAY_BUFFER,
        verticesWall.size() * sizeof(Vertex3D),
        verticesWall.data(),
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

    static constexpr GLuint aVertexTexCoords = 2;
    glEnableVertexAttribArray(aVertexTexCoords);
    glVertexAttribPointer(
        aVertexTexCoords, 2, GL_FLOAT, GL_FALSE,
        sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords))
    );
    vbo.unbind();
    vao.unbind();

    VAO vaoWall;
    vaoWall.bind();
    // Activation vertex
    vboWall.bind();
    static constexpr GLuint aVertexPositionWall = 0;
    glEnableVertexAttribArray(aVertexPositionWall);
    glVertexAttribPointer(
        aVertexPositionWall, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, position))
    );
    static constexpr GLuint aVertexTexCoordsWall = 2;
    glEnableVertexAttribArray(aVertexTexCoordsWall);
    glVertexAttribPointer(
        aVertexTexCoordsWall, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, texture))
    );
    vboWall.unbind();
    vaoWall.unbind();

    // ---

    glEnable(GL_DEPTH_TEST); // active le test de profondeur du GPU

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
        ctx.background(p6::NamedColor::CeladonBlue);

        // TODO cube
        // ctx.square(p6::Center{0., 0.}, p6::Radius{0.8f}, p6::Rotation{0.0_turn});

        groupe.draw(
            ctx, vao, verticesSphere,
            wings, eyes, body, textures,
            camera.getViewMatrix()
        );

        groupe.animate(
            ctx,
            options.align,
            options.separate, options.cohesion,
            options.coeffAlignement, options.coeffRepulsion, options.coeffCohesion,
            ctx.delta_time()
        );

        // Shader
        // TODO glimac::bind_default_shader();
        shader.use();
        // GLuint uTexture = glGetUniformLocation(shader.id(), "uTexture");
        glBindTexture(GL_TEXTURE_2D, textureWall);
        shader.bindTexture(0);
        // glUniform1i(uTexture, 0);

        // Bind VAO
        vaoWall.bind();

        glm::mat4 MVMatrix = camera.getViewMatrix();

        // --- Sol
        MVMatrix = glm::translate(MVMatrix, glm::vec3{0.f, 0.f, -2.f});
        MVMatrix = glm::rotate(MVMatrix, glm::degrees(90.f), glm::vec3(1., 0., 0.));
        MVMatrix = glm::scale(MVMatrix, glm::vec3(6., 6., 1.));
        shader.giveMatrix(ctx, MVMatrix);
        // TODO utiliser vertex grass dans le main
        glDrawArrays(GL_TRIANGLES, 0, verticesWall.size()); // TODO render

        // --- Plafond
        MVMatrix = glm::translate(MVMatrix, glm::vec3{0.f, 0.f, -fond * 3.});
        shader.giveMatrix(ctx, MVMatrix);
        glDrawArrays(GL_TRIANGLES, 0, verticesWall.size()); // TODO render

        // TODO les autres murs
        // shader.giveMatrix(ctx, MVMatrix);
        // glDrawArrays(GL_TRIANGLES, 0, verticesWall.size()); // TODO render

        vaoWall.unbind();

        // TODO adapter le nb de vertices en fonction de la taille qu'elle représente ?
        beez.draw(
            ctx, vao, verticesSphere,
            wings, eyes, body, textures,
            glm::vec3(ctx.mouse() * ctx.aspect_ratio() * (1.5f + 0.5f / 2.f), -5.),
            glm::vec3(0.3), camera.getViewMatrix()
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}