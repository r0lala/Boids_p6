#include <string>
#include "p6/p6.h"

class Texture
{
private:
    p6::Shader _program;
    GLint _uMVPMatrix;
    GLint _uMVMatrix;
    GLint _uNormalMatrix;

    // TODO cas où faut traiter plusieurs textures
    GLint _uTexture;

public:
    Texture(std::string vertex, std::string fragment);
    ~Texture();
    void use();
    void giveMatrix(glm::mat4 ProjMatrix, glm::mat4 MVMatrix, glm::mat4 NormalMatrix);
    void bind();
};