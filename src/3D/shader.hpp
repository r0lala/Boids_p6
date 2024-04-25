#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include "p6/p6.h"
#include "vao.hpp"
#include "GLIMAC/sphere_vertices.hpp"

class Shader {
private:
    p6::Shader _program;
    GLint      _uMVPMatrix;
    GLint      _uMVMatrix;
    GLint      _uNormalMatrix;

    // TODO cas où faut traiter plusieurs textures
    GLint _uTexture;

public:
    Shader(std::string vertex, std::string fragment);
    // ~Shader();
    void use();
    void giveMatrix(p6::Context& ctx, glm::mat4 ViewMatrix);
    void bindTexture(int textUnit);
    void render(
        VAO& vao, 
        const std::vector<glimac::ShapeVertex>& vertices,
        GLuint textures = -1, int textUnit = -1
    );
};

#endif