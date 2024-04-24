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
    float angle
)
{
    float     z        = angle > 0. ? 0.5 : -0.5;
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), {0.1f, 1.4f, z});
    MVMatrix           = glm::rotate(MVMatrix, angle, glm::vec3{1.f, 0.f, 0.f});
    MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.5f, 1.f, 0.1f});
    // scale 0.8f ?

    return MVMatrix;
}

// TODO donner un angle ?
glm::mat4 Bee::giveFace(
    float angle
)
{
    float z = angle > 0. ? 0.5 : -0.5;

    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-0.9, 0, z));
    MVMatrix           = glm::scale(MVMatrix, glm::vec3{0.18f});

    return MVMatrix;
};

// TODO direction ?
glm::mat4 Bee::giveBody()
{
    glm::mat4 MVMatrix = glm::mat4(1);
    MVMatrix           = glm::translate(MVMatrix, glm::vec3(0, 0, -5));
    return MVMatrix;
}

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

void Bee::draw(
    p6::Context& ctx, VAO& vao,
    const std::vector<glimac::ShapeVertex>& vertices,
    Shader& wings, Shader& eyes, Shader& body, GLuint textures, // TODO supp param shader + texture
    glm::vec3 position, glm::vec3 scale
)
{
    glm::mat4 MVMatrix = this->giveBody();
    MVMatrix           = glm::translate(MVMatrix, position);
    MVMatrix           = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f}); // TODO debug
    // TODO scale ?
    MVMatrix = glm::scale(MVMatrix, scale);

    body.use();
    body.giveMatrix(ctx, MVMatrix);
    this->render(vao, vertices, body, textures, 0);

    // TODO regrouper ctx et vao ?
    // TODO angle = à quel point on fronce les yeux de l'abeille ?
    eyes.use();
    eyes.giveMatrix(ctx, MVMatrix * this->giveFace(1.f));
    this->render(vao, vertices, eyes);
    eyes.giveMatrix(ctx, MVMatrix * this->giveFace(-1.f));
    this->render(vao, vertices, eyes);

    // TODO regrouper en drawWings ? => boucle for ?
    wings.use();
    wings.giveMatrix(ctx, MVMatrix * this->giveWing(35.f));
    this->render(vao, vertices, wings);

    wings.giveMatrix(ctx, MVMatrix * this->giveWing(-35.f));
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