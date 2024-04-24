#include "bee.hpp"

// TODO => créer nos propres fct de matrice ?
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "3D/vecTextures.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"

// #include "3D/glimac/sphere_vertices.hpp"
// void move(glm::vec3 acceleration, float delta_time);

glm::mat4 Bee::giveWing(
    p6::Context& ctx,
    float        angle
)
{
    float z = angle > 0. ? 0.5 : -0.5;
    // TODO faire en sorte qu'il ne dépend pas d'une autre matrice ?
    glm::mat4 MVMatrix = glm::translate(this->giveBody(ctx), {0.1f, 1.4f, z});
    MVMatrix           = glm::rotate(MVMatrix, angle, glm::vec3{1.f, 0.f, 0.f});

    // TODO moyen de regrouper les scales ?
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.8f});
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f, 1.f, 0.1f});

    return MVMatrix;
}

// TODO donner un angle ?
void Bee::giveFace(
    p6::Context& ctx, VAO& vao, Shader& eyes, const std::vector<glimac::ShapeVertex>& vertices
)
{
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix           = glm::scale(
        glm::translate(
            MVMatrix,
            {-0.5f, 0.f, 0.25f}
        ),
        glm::vec3{0.1f}
    );

    eyes.use();
    eyes.giveMatrix(ctx, MVMatrix);
    this->render(vao, vertices, eyes);

    // Second eye
    MVMatrix = glm::translate(
        MVMatrix,
        {0.f, 0.f, -5.f}
    );

    eyes.giveMatrix(ctx, MVMatrix);
    this->render(vao, vertices, eyes);
};

glm::mat4 Bee::giveBody(p6::Context& ctx)
{
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.6, 0.5f, 0.5});

    return MVMatrix;
}

// TODO rename => render ?
void Bee::render(VAO& vao, const std::vector<glimac::ShapeVertex>& vertices, Shader& shader, const Texture& texture, int textUnit)
{
    vao.bind();
    if (textUnit >= 0)
    {
        texture.bind();
        // glBindTexture(GL_TEXTURE_2D, texture);
        shader.bindTexture(textUnit);
    }
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    if (textUnit >= 0)
    {
        texture.unbind();
        // glBindTexture(GL_TEXTURE_2D, 0);
    }
    vao.unbind();
}

// TODO trouver un meilleur nom
void Bee::draw(
    p6::Context& ctx, VAO& vao,
    const std::vector<glimac::ShapeVertex>& vertices,
    Shader& wings, Shader& eyes, Shader& body, const Texture& texture // TODO supp param shader + texture
)
{
    body.use();
    body.giveMatrix(ctx, this->giveBody(ctx));
    this->render(vao, vertices, body, texture._id, 0);

    // TODO regrouper ctx et vao ?
    // TODO dépendance avec le body
    this->giveFace(ctx, vao, eyes, vertices);

    // TODO regrouper en drawWings ? => boucle for ?
    wings.use();
    wings.giveMatrix(ctx, this->giveWing(ctx, 35.f));
    this->render(vao, vertices, wings);

    wings.giveMatrix(ctx, this->giveWing(ctx, -35.f));
    this->render(vao, vertices, wings);
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