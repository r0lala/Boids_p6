// TODO faire le tri des includes ...
#include <cstdlib>
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
#include "3D/glimac/sphere_vertices.hpp"
#include "3D/vao.hpp"
#include "3D/vbo.hpp"
#include "boids/swarm.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"
#include "param/options.hpp"
#include "random/rand.hpp"
#include "3D/shader.hpp"

int main()
{
    // TODO : changer la direction pour qu'elle soit aléatoire
    Swarm groupe(50);
    srand(time(NULL)); // TODO à déplacer ?

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
    img::Image triforce = p6::load_image_buffer("../assets/textures/MoonMap.jpg");
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

    // Texture  
    // TODO dans un fichier
    GLuint textures;
    glGenTextures(1, &textures);
    glBindTexture(GL_TEXTURE_2D, textures);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, 
        triforce.width(), triforce.height(), 
        0, GL_RGBA, GL_UNSIGNED_BYTE, triforce.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

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
        body.use();

        // Bind VAO
        vao.bind();

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
        // MVMatrix = glm::rotate(MVMatrix, 90.f, {0.f, 0.f, 0.f});
        MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
        MVMatrix = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        body.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);

        // Draw triangle
        glBindTexture(GL_TEXTURE_2D, textures);
        body.bindTexture(0);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindTexture(GL_TEXTURE_2D, 0);
        vao.unbind();

        // TODO autres spheres / parties de l'abeille
        vao.bind();
            eyes.use();

            MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});

            MVMatrix = glm::scale(
                        glm::translate(
                            MVMatrix, 
                            {-0.5f, 0.f, 0.25f}), 
                        glm::vec3{0.1f}
                    );
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            body.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.unbind();
        vao.bind();
            eyes.use();

            MVMatrix = glm::translate(
                            MVMatrix, 
                            {0.f, 0.f, -4.5f});
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        body.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.unbind();

        vao.bind();
        // TODO à revoir pour l'inclinaison de l'aile
        // TODO faire la 2e aile
            wings.use();
            MVMatrix = glm::scale(
                            glm::translate(
                                glm::mat4(1), 
                                {0.f, 0.8f, -5.f}), 
                    glm::vec3{0.5f, 0.75, 0.5}
                );
            MVMatrix = glm::scale(
                MVMatrix, glm::vec3{0.5f}
            );
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), glm::vec3{1.f, 0.f, 0.f});
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            body.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteTextures(1, &textures);
    return EXIT_SUCCESS;
}