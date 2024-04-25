#ifndef __VAO_H__
#define __VAO_H__

#include "glm/glm.hpp"
#include "p6/p6.h"

class VAO {
private:
    GLuint _id{};

public:
    VAO()
    {
        glGenVertexArrays(1, &_id);
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &_id);
    }
    VAO(const VAO& other)            = delete;
    VAO& operator=(const VAO& other) = delete;
    VAO(VAO&& other) noexcept // Move constructor
        : _id{other._id}
    {
        other._id = 0; // Make sure that other won't delete the _id we just copied
    }

    VAO& operator=(VAO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                                  // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteVertexArrays(1, &_id); // Delete the previous object
            _id       = other._id;         // Copy the object
            other._id = 0;                 // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    void bind() const
    {
        glBindVertexArray(_id);
    }

    void unbind() const
    {
        glBindVertexArray(0);
    }
};

#endif