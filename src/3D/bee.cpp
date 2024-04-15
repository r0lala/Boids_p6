#include "bee.hpp"

// TODO => créer nos propres fct de matrice ?
#include "3D/shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"

// #include "3D/glimac/sphere_vertices.hpp"
// void move(glm::vec3 acceleration, float delta_time);
void Bee::drawWing(
    p6::Context& ctx, float angle,
    VAO& vao, glm::mat4& bodyMatrix, Shader& wings,
    const std::vector<glimac::ShapeVertex>& vertices
)
{
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    float     z        = angle > 0. ? 0.5 : -0.5;
    glm::mat4 MVMatrix = glm::translate(bodyMatrix, {0.1f, 1.4f, z});
    MVMatrix           = glm::rotate(MVMatrix, angle, glm::vec3{1.f, 0.f, 0.f});

    // TODO moyen de regrouper les scales ?
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.8f});
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f, 1.f, 0.1f});

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    vao.bind();
    wings.use();
    wings.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    vao.unbind();
}

void Bee::drawFace(
    p6::Context& ctx, VAO& vao, Shader& eyes, const std::vector<glimac::ShapeVertex>& vertices
)
{
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    glm::mat4 MVMatrix   = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    MVMatrix             = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix             = glm::scale(
        glm::translate(
            MVMatrix,
            {-0.5f, 0.f, 0.25f}
        ),
        glm::vec3{0.1f}
    );

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    vao.bind();
    eyes.use();
    eyes.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    MVMatrix = glm::translate(
        MVMatrix,
        {0.f, 0.f, -5.f}
    );
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    eyes.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    vao.unbind();
};

void Bee::drawBody(Shader& body, VAO& vao, p6::Context& ctx, const std::vector<glimac::ShapeVertex>& vertices, GLuint textures)
{
    // Shader
    body.use();

    // Bind VAO
    vao.bind();

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    glm::mat4 MVMatrix   = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    // MVMatrix = glm::rotate(MVMatrix, 90.f, {0.f, 0.f, 0.f});
    MVMatrix               = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix               = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glm::mat4 bodyMatrix   = MVMatrix;
    body.giveMatrix(ProjMatrix, MVMatrix, NormalMatrix); // TODO réduire le nombre de param

    // Draw triangle
    glBindTexture(GL_TEXTURE_2D, textures);
    body.bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindTexture(GL_TEXTURE_2D, 0);
    vao.unbind();
}

// void drawBee(p6::Context& ctx) const
// {
// }

// void initBee(VBO& vbo, VAO& vao)
// {
//     // Activation vertex
//     vbo.bind();
//     static constexpr GLuint aVertexPosition = 0;
//     glEnableVertexAttribArray(aVertexPosition);
//     glVertexAttribPointer(
//         aVertexPosition, 3, GL_FLOAT, GL_FALSE,
//         sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position))
//     );

//     static constexpr GLuint aVertexNormal = 1;
//     glEnableVertexAttribArray(aVertexNormal);
//     glVertexAttribPointer(
//         aVertexNormal, 3, GL_FLOAT, GL_FALSE,
//         sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal))
//     );

//     static constexpr GLuint aVertexTexCoords = 2;
//     glEnableVertexAttribArray(aVertexTexCoords);
//     glVertexAttribPointer(
//         aVertexTexCoords, 2, GL_FLOAT, GL_FALSE,
//         sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords))
//     );
//     vbo.unbind();
//     vao.unbind();
// }