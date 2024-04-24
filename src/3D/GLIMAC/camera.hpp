#include "common.hpp"
#include "glm/ext/matrix_clip_space.hpp"

class Camera {
private:
    float _distance = -5;
    float _angleX   = 0;
    float _angleY   = 0;

public:
    void moveFront(float delta)
    {
        _distance += delta;
    }

    void rotateLeft(float degrees)
    {
        _angleX += degrees;
    }

    void rotateUp(float degrees)
    {
        _angleY += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 transform = glm::translate(glm::mat4{1.f}, {0.f, 0.f, this->_distance});
        transform           = glm::rotate(transform, -this->_angleX, {1.f, 0.f, 0.f});
        transform           = glm::rotate(transform, -this->_angleY, {0.f, 1.f, 0.f});
        return transform;
    }
};