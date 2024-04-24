#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "p6/p6.h"

class vecTextures {
private:
    GLuint              _nbTextures = 8;
    std::vector<GLuint> _textures   = {0, 0, 0, 0, 0, 0, 0, 0}; // TODO

public:
    vecTextures()
    {
        glGenTextures(this->_nbTextures, this->_textures.data());
    }; // 1 = nombre de textures

    ~vecTextures()
    {
        glDeleteTextures(this->_nbTextures, this->_textures.data());
    };

    GLuint getIndexTexture(GLuint index)
    {
        return _textures[index];
    }
};