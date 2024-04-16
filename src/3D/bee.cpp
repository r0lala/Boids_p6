#include "bee.hpp"

// TODO => créer nos propres fct de matrice ?
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"

// #include "3D/glimac/sphere_vertices.hpp"
// void move(glm::vec3 acceleration, float delta_time);

void Bee::giveWing(
    p6::Context& ctx,
    float        angle,
    glm::mat4&   bodyMatrix,
    Shader&      wings
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

    // TODO objectif de séparation ---
    wings.use();
    wings.giveMatrix(ctx, MVMatrix);
}

// TODO donner un angle ?
void Bee::giveFace(
    p6::Context& ctx, VAO& vao, Shader& eyes, const std::vector<glimac::ShapeVertex>& vertices
)
{
    eyes.use();

    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix           = glm::scale(
        glm::translate(
            MVMatrix,
            {-0.5f, 0.f, 0.25f}
        ),
        glm::vec3{0.1f}
    );

    eyes.giveMatrix(ctx, MVMatrix);
    this->draw(vao, vertices, eyes);

    // Second eye
    MVMatrix = glm::translate(
        MVMatrix,
        {0.f, 0.f, -5.f}
    );

    eyes.giveMatrix(ctx, MVMatrix);
    this->draw(vao, vertices, eyes);
};

void Bee::giveBody(p6::Context& ctx, Shader& body)
{
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});

    body.use();
    body.giveMatrix(ctx, MVMatrix);
}

// TODO rename => render ?
void Bee::draw(VAO& vao, const std::vector<glimac::ShapeVertex>& vertices, Shader& shader, GLuint textures, int textUnit)
{
    vao.bind();
    if (textUnit >= 0)
    {
        glBindTexture(GL_TEXTURE_2D, textures);
        shader.bindTexture(textUnit);
    }
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    if (textUnit >= 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    vao.unbind();
}

// TODO trouver un meilleur nom
void Bee::drawBee(
    p6::Context& ctx, VAO& vao,
    const std::vector<glimac::ShapeVertex>& vertices,
    Shader& wings, Shader& eyes, Shader& body, GLuint textures // TODO supp param shader + texture
)
{
    // TODO regrouper dans une sous fonction de drawBody
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});

    this->giveBody(ctx, body);
    this->draw(vao, vertices, body, textures, 0);

    // TODO regrouper ctx et vao ?
    // TODO dépendance avec le body
    this->giveFace(ctx, vao, eyes, vertices);

    // TODO regrouper en drawWings ? => boucle for ?
    this->giveWing(ctx, 35.f, MVMatrix, wings);
    this->draw(vao, vertices, wings);

    this->giveWing(ctx, -35.f, MVMatrix, wings);
    this->draw(vao, vertices, wings);
}

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