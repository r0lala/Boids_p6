#include "shader.hpp"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(std::string vertex, std::string fragment)
    : _program{p6::load_shader("../src/3D/shaders/" + vertex + ".vs.glsl", "../src/3D/shaders/" + fragment + ".fs.glsl")}
{
    _uMVPMatrix    = glGetUniformLocation(_program.id(), "uMVPMatrix");
    _uMVMatrix     = glGetUniformLocation(_program.id(), "uMVMatrix");
    _uNormalMatrix = glGetUniformLocation(_program.id(), "uNormalMatrix");
    _uTexture      = glGetUniformLocation(_program.id(), "uTexture");
}

Shader::~Shader()
{
    // glDeleteTextures(1, &textures);
}

void Shader::use()
{
    _program.use();
}

void Shader::giveMatrix(p6::Context& ctx, glm::mat4 MVMatrix)
{
    glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // TODO : seulement MVMatrix suffit ? le reste ne change pas
    glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}

void Shader::bindTexture(int textUnit)
{
    glUniform1i(_uTexture, textUnit);
}