#include "default_shader.hpp"
#include "glad/gl.h"

namespace glimac {

static unsigned int create_default_shader()
{
    const char* const vertexShaderSource = R"STR(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        
        void main()
        {
           gl_Position = vec4(aPos, 0., 1.);
        }
    )STR";

    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    const unsigned int fragmentShader      = glCreateShader(GL_FRAGMENT_SHADER);
    const char* const  framentShaderSource = R"STR(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.f);
        }
    )STR";
    glShaderSource(fragmentShader, 1, &framentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void bind_default_shader()
{
    static unsigned int default_shader = create_default_shader();
    glUseProgram(default_shader);
}

} // namespace glimac