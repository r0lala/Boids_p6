#include <glm/glm.hpp>
#include "GLIMAC/default_shader.hpp"

class VBO {
private:
    GLuint _id{};

public:
    VBO()
    {
        glGenBuffers(1, &_id);
    }
    ~VBO()
    {
        glDeleteBuffers(1, &_id);
    }
    VBO(const VBO& other)            = delete;
    VBO& operator=(const VBO& other) = delete;
    VBO(VBO&& other) noexcept // Move constructor
        : _id{other._id}
    {
        other._id = 0; // Make sure that other won't delete the _id we just copied
    }

    VBO& operator=(VBO&& other) noexcept // Move assignment operator
    {
        if (this != &other)
        {                             // Make sure that we don't do silly things when we try to move an object to itself
            glDeleteBuffers(1, &_id); // Delete the previous object
            _id       = other._id;    // Copy the object
            other._id = 0;            // Make sure that other won't delete the _id we just copied
        }
        return *this; // move assignment must return a reference to this, so we do it
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};