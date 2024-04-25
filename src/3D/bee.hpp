#pragma once

#include <vector>
#include "3D/glimac/common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "p6/p6.h"
#include "shader.hpp"
#include "vao.hpp"

class Bee {
private:
    // Shader body("3D", "bee/body");
    // Shader eyes("3D", "bee/eyes");
    // Shader wings("3D", "bee/wings");
    glm::mat4 giveWing(float angle);
    glm::mat4 giveFace(float angle);
    glm::mat4 giveBody();

public:
    Bee(){};
    void draw(
        p6::Context& ctx, VAO& vao,
        const std::vector<glimac::ShapeVertex>& vertices,
        Shader& wings, Shader& eyes, Shader& body, GLuint textures,
        glm::vec3 position, glm::vec3 scale,
        glm::mat4 ViewMatrix, float angle, glm::vec3 axe
    );
};