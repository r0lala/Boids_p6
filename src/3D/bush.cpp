#include "bush.hpp"
#include <glm/ext/matrix_clip_space.hpp> // TODO à coder nous même ?
#include <glm/ext/matrix_transform.hpp> // TODO à coder nous même ?

void Bush::draw(
// TODO ctx pas nécessaire ? à stocker directement dans shader ?
    p6::Context &ctx,
    VAO &vao, Shader &tree,
    const std::vector<glimac::ShapeVertex>& vertices,
    const glm::mat4 &viewMatrix, GLuint textures
){
// TODO découper en sous fonction ? boucle for ?
// part 1
    glm::mat4 MVMatrix = glm::scale(viewMatrix, glm::vec3(0.8f, 0.6f, 0.8f));

    tree.use();

    vao.bind();
    tree.giveMatrix(ctx, MVMatrix);

    glBindTexture(GL_TEXTURE_2D, textures);
    tree.bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    // glBindTexture(GL_TEXTURE_2D, 0); // TODO à remettre ?

    vao.unbind();

// part 2
    vao.bind();
    MVMatrix = viewMatrix;
    MVMatrix = glm::scale(MVMatrix, glm::vec3(1.2f, 1.f, 1.2f));
    MVMatrix = glm::scale(
        glm::translate(
            MVMatrix,
            {0.8f, 0.f, 0.25f}
        ),
        glm::vec3{0.6f}
    );

    tree.giveMatrix(ctx, MVMatrix);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    vao.unbind();

// part 3
    vao.bind();

    MVMatrix = viewMatrix;
    MVMatrix = glm::scale(MVMatrix, glm::vec3(1.2f, 1.f, 1.2f));
    MVMatrix = glm::scale(
        glm::translate(
            MVMatrix,
            {0.5f, 0.8f, 0.f}
        ),
        glm::vec3{0.6f}
    );

    tree.giveMatrix(ctx, MVMatrix);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    vao.unbind();

// part 4
    vao.bind();

    MVMatrix = viewMatrix;
    MVMatrix = glm::scale(MVMatrix, glm::vec3(1.2f, 1.f, 1.2f));
    MVMatrix = glm::scale(
        glm::translate(
            MVMatrix,
            {0.f, 0.6f, 0.1f}
        ),
        glm::vec3{0.3f}
    );

    tree.giveMatrix(ctx, MVMatrix);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    vao.unbind();

// part 5
    vao.bind();

    MVMatrix = viewMatrix;
    MVMatrix = glm::scale(MVMatrix, glm::vec3(1.2f, 1.f, 1.2f));
    MVMatrix = glm::scale(
        glm::translate(
            MVMatrix,
            {0.4f, 0.4f, 0.4f}
        ),
        glm::vec3{0.3f}
    );

    tree.giveMatrix(ctx, MVMatrix);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindTexture(GL_TEXTURE_2D, 0);
    vao.unbind();
}