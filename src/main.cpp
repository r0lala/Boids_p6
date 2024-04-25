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
    const p6::Shader shader = p6::load_shader(
        "../src/3D/shaders/wall.vs.glsl",
        "../src/3D/shaders/wall.fs.glsl"
    );

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
    const std::vector<glimac::ShapeVertex> vertices_sphere = glimac::sphere_vertices(1.f, 32, 16);
    Vertex3D                               vertices[]      = {
        Vertex3D{{0.5f, -0.5f, -2}, {1.f, 0.f}},
        Vertex3D{{-0.5f, 0.5f, -2}, {1.f, 0.f}},
        Vertex3D{{-0.5f, -0.5f, -2}, {0.5f, 0.5f}},
        Vertex3D{{0.5f, -0.5f, -2}, {1.f, 0.f}},
        Vertex3D{{0.5f, 0.5f, -2}, {0.f, 1.f}},
        Vertex3D{{-0.5f, 0.5f, -2}, {0.f, 1.f}}
    };

    // Sending the data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices_sphere.size() * sizeof(glimac::ShapeVertex),
        vertices_sphere.data(),
        GL_STATIC_DRAW
    );

    vbo.unbind();

    VBO vbo2;
    vbo2.bind();

    Vertex3D vertices2[] = {
        // Vertex3D{{0.5f, -0.5f, 4}, {1.f, 0.f}},
        // Vertex3D{{-0.5f, 0.5f, 4}, {1.f, 0.f}},
        // Vertex3D{{-0.5f, -0.5f, 4}, {0.5f, 0.5f}},
        // Vertex3D{{0.5f, -0.5f, 4}, {1.f, 0.f}},
        // Vertex3D{{0.5f, 0.5f, 4}, {0.f, 1.f}},
        // Vertex3D{{-0.5f, 0.5f, 4}, {0.f, 1.f}}
        Vertex3D{{0.5f, -0.5f, -2}, {1.f, 0.f}},
        Vertex3D{{-0.5f, 0.5f, -2}, {1.f, 0.f}},
        Vertex3D{{-0.5f, -0.5f, -2}, {0.5f, 0.5f}},
        Vertex3D{{0.5f, -0.5f, -2}, {1.f, 0.f}},
        Vertex3D{{0.5f, 0.5f, -2}, {0.f, 1.f}},
        Vertex3D{{-0.5f, 0.5f, -2}, {0.f, 1.f}}
    };

    glBufferData(
        GL_ARRAY_BUFFER,
        6 * sizeof(Vertex3D),
        vertices2,
        GL_STATIC_DRAW
    );
    vbo2.unbind();

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
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, position))
    );

    static constexpr GLuint aVertexTexCoords = 2;
    glEnableVertexAttribArray(aVertexTexCoords);
    glVertexAttribPointer(
        aVertexTexCoords, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, texture))
    );
    vbo.unbind();
    vao.unbind();

    VAO vao2;
    vao2.bind();
    // Activation vertex
    vbo2.bind();
    static constexpr GLuint aVertexPosition_2 = 0;
    glEnableVertexAttribArray(aVertexPosition_2);
    glVertexAttribPointer(
        aVertexPosition_2, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, position))
    );
    static constexpr GLuint aVertexTexCoords_2 = 2;
    glEnableVertexAttribArray(aVertexTexCoords_2);
    glVertexAttribPointer(
        aVertexTexCoords_2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex3D), (const GLvoid*)(offsetof(Vertex3D, texture))
    );
    vbo2.unbind();
    vao2.unbind();

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
            ctx, vao, vertices_sphere,
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
        GLuint uTexture = glGetUniformLocation(shader.id(), "uTexture");
        glBindTexture(GL_TEXTURE_2D, textureWall);
        glUniform1i(uTexture, 0);

        // Bind VAO
        vao.bind();
        // Draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Unbind vao
        vao.unbind();

        vao2.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao2.unbind();
        // TODO adapter le nb de vertices en fonction de la taille qu'elle représente ?
        beez.draw(
            ctx, vao, vertices_sphere,
            wings, eyes, body, textures,
            glm::vec3(ctx.mouse() * ctx.aspect_ratio() * (1.5f + 0.5f / 2.f), -5.),
            glm::vec3(0.3), camera.getViewMatrix()
        );
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}