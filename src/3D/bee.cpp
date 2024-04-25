#include "bee.hpp"

// TODO => créer nos propres fct de matrice ?
#include "3D/shader.hpp"
#include "3D/vao.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"

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

glm::mat4 Bee::giveBody()
{
    return glm::mat4(1); // MVMatrix
}

void Bee::draw(
    p6::Context& ctx, VAO& vao,
    const std::vector<glimac::ShapeVertex>& vertices,
    Shader& wings, Shader& eyes, Shader& body, GLuint textures, // TODO supp param shader + texture
    glm::vec3 position, glm::vec3 scale,
    glm::mat4 ViewMatrix, float angle, glm::vec3 axe
)
{
    glm::mat4 MVMatrix = ViewMatrix * this->giveBody();
    MVMatrix           = glm::translate(MVMatrix, position);
    MVMatrix           = glm::rotate(MVMatrix, angle, axe); // TODO debug
    MVMatrix           = glm::scale(MVMatrix, scale);

    body.use();
    body.giveMatrix(ctx, MVMatrix);

    body.render(vao, vertices, textures, 0);

    // TODO angle = à quel point on fronce les yeux de l'abeille ?
    eyes.use();
    eyes.giveMatrix(ctx, MVMatrix * this->giveFace(1.f));

    eyes.render(vao, vertices);
    eyes.giveMatrix(ctx, MVMatrix * this->giveFace(-1.f));
    eyes.render(vao, vertices);

    // TODO regrouper en drawWings ? => boucle for ?
    wings.use();
    wings.giveMatrix(ctx, MVMatrix * this->giveWing(35.f));

    wings.render(vao, vertices);

    wings.giveMatrix(ctx, MVMatrix * this->giveWing(-35.f));
    wings.render(vao, vertices);
}