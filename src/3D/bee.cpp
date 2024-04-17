#include "bee.hpp"

// TODO => créer nos propres fct de matrice ?
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
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
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), {0.1f, 1.4f, z});
    MVMatrix           = glm::rotate(MVMatrix, angle, glm::vec3{1.f, 0.f, 0.f});

    // TODO moyen de regrouper les scales ?
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f, 1.f, 0.1f}); // 0.8f
    // MVMatrix = glm::scale(MVMatrix, glm::vec3{0.5f, 1.f, 0.1f});

    return MVMatrix;
}

// TODO donner un angle ?
glm::mat4 Bee::giveFace(
    p6::Context& ctx, VAO& vao, Shader& eyes, const std::vector<glimac::ShapeVertex>& vertices,
    float angle
)
{
    float z = angle > 0. ? 0.5 : -0.5;

    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.9, 0, z));
    // MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    // MVMatrix = glm::translate(MVMatrix, {5.f, 0.f, 0.25f}); // TODO z à modifier
    MVMatrix = glm::scale(MVMatrix, glm::vec3{0.15f});

    return MVMatrix;

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
    glm::mat4 MVMatrix = glm::mat4(1);
    MVMatrix           = glm::translate(MVMatrix, glm::vec3(0, 0, -5));
    MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.5f, 0.5f, 0.5f});

    return MVMatrix;
}

// TODO rename => render ?
void Bee::render(VAO& vao, const std::vector<glimac::ShapeVertex>& vertices, Shader& shader, GLuint textures, int textUnit)
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
void Bee::draw(
    p6::Context& ctx, VAO& vao,
    const std::vector<glimac::ShapeVertex>& vertices,
    Shader& wings, Shader& eyes, Shader& body, GLuint textures, // TODO supp param shader + texture
    // glm::mat4 scale
    glm::vec3 position
)
{
    glm::mat4 MVMatrix = this->giveBody(ctx);
    // MVMatrix           = glm::translate(MVMatrix, position);
    MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f}); // TODO debug
    // TODO scale ?

    body.use();
    body.giveMatrix(ctx, MVMatrix);
    this->render(vao, vertices, body, textures, 0);

    // TODO regrouper ctx et vao ?
    // TODO à quel point on fronce les yeux de l'abeille ?
    eyes.use();
    eyes.giveMatrix(ctx, MVMatrix * this->giveFace(ctx, vao, eyes, vertices, 1.f));
    this->render(vao, vertices, eyes);
    eyes.giveMatrix(ctx, MVMatrix * this->giveFace(ctx, vao, eyes, vertices, -1.f));
    this->render(vao, vertices, eyes);

    // TODO regrouper en drawWings ? => boucle for ?
    wings.use();
    wings.giveMatrix(ctx, MVMatrix * this->giveWing(ctx, 35.f));
    this->render(vao, vertices, wings);

    wings.giveMatrix(ctx, MVMatrix * this->giveWing(ctx, -35.f));
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