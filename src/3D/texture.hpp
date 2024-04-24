#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "3D/glimac/common.hpp"
#include "p6/p6.h"

class Texture {
private:
    GLuint     _id{};
    img::Image _img;

public:
    Texture(Gluint id, std::string path)
    {
        this->_id  = id;
        this->_img = p6::load_image_buffer(path, false);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            this->_img.width(), this->_img.height(),
            0, GL_RGBA, GL_UNSIGNED_BYTE, this->_img.data()
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    // ~Texture(){}; //TODO
};
