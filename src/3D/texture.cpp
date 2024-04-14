#include "texture.hpp"
#include "glm/gtc/type_ptr.hpp"

Texture::Texture(std::string vertex, std::string fragment)
: _program{p6::load_shader("../src/3D/shaders/" + vertex + ".vs.glsl",
            "../src/3D/shaders/" + fragment + ".fs.glsl")}
{
    _uMVPMatrix    = glGetUniformLocation(_program.id(), "uMVPMatrix");
    _uMVMatrix     = glGetUniformLocation(_program.id(), "uMVMatrix");
    _uNormalMatrix = glGetUniformLocation(_program.id(), "uNormalMatrix");
    _uTexture      = glGetUniformLocation(_program.id(), "uTexture");
}

Texture::~Texture()
{
    // glDeleteTextures(1, &textures);
}

void Texture::use(){
    _program.use();
}

void Texture::giveMatrix(glm::mat4 ProjMatrix, glm::mat4 MVMatrix, glm::mat4 NormalMatrix){
    glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}

void Texture::bind(){
    glUniform1i(_uTexture, 0);
}