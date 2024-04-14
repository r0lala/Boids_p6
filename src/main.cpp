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
    const p6::Shader shader = p6::load_shader(
        "../src/3D/shaders/3D.vs.glsl",
        "../src/3D/shaders/bee/body.fs.glsl"
    );
    const p6::Shader eyes = p6::load_shader(
        "../src/3D/shaders/3D.vs.glsl",
        "../src/3D/shaders/bee/eyes.fs.glsl"
    );
    const p6::Shader wings = p6::load_shader(
        "../src/3D/shaders/3D.vs.glsl",
        "../src/3D/shaders/bee/wings.fs.glsl"
        // "../src/3D/shaders/normals.fs.glsl"
    );

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

    // Variables uniformes
    GLint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");
    GLuint uTexture = glGetUniformLocation(shader.id(), "uTexture");

    // Texture  
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
        shader.use();

        // Bind VAO
        vao.bind();
        // uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        // uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
        // uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
        // MVMatrix = glm::rotate(MVMatrix, 90.f, {0.f, 0.f, 0.f});
        MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
        MVMatrix = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        // Draw triangle
        // glUniform3fv(uColorLocation, 1, glm::value_ptr(glm::vec3(i / 2., (i + j) / 4., 0.5)));
        glBindTexture(GL_TEXTURE_2D, textures);
        glUniform1i(uTexture, 0);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindTexture(GL_TEXTURE_2D, 0);
        // Unbind vao
        vao.unbind();

        // TODO autres spheres / parties de l'abeille
        vao.bind();

            // uMVPMatrixLocation    = glGetUniformLocation(eyes.id(), "uMVPMatrix");
            // uMVMatrixLocation     = glGetUniformLocation(eyes.id(), "uMVMatrix");
            // uNormalMatrixLocation = glGetUniformLocation(eyes.id(), "uNormalMatrix");
            eyes.use();

            MVMatrix     = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
            MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});

            MVMatrix = glm::scale(
                        glm::translate(
                            MVMatrix, 
                            {-0.5f, 0.f, 0.25f}), 
                        glm::vec3{0.1f}
                    );
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            // glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.unbind();
        vao.bind();
            eyes.use();

            MVMatrix = glm::translate(
                            MVMatrix, 
                            {0.f, 0.f, -4.5f});
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));

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

            // TODO => une fonction
            glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            // glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        vao.unbind();
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteTextures(1, &textures);
    return EXIT_SUCCESS;
}