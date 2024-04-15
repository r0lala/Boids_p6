#include "common.hpp"
#include "glm/ext/matrix_clip_space.hpp"

class Camera {
private:
    float m_Distance;
    float m_AngleX;
    float m_AngleY;

public:
    void moveFront(float delta)
    {
        if (delta > 0)
        {
            m_Distance += delta;
        }
        else
        {
            m_Distance -= delta;
        }
    }

    void rotateLeft(float degrees)
    {
        m_AngleX += degrees;
    }

    void rotateUp(float degrees)
    {
        m_AngleY += degrees;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 transform = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -this->m_Distance});
        transform           = glm::rotate(transform, glm::radians(-this->m_AngleX), {1.f, 0.f, 0.f});
        transform           = glm::rotate(transform, glm::radians(-this->m_AngleY), {0.f, 1.f, 0.f});
        return transform;
    }
};