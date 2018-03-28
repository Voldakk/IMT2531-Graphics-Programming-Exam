#include "Light.hpp"

void EVA::Light::SetRotation(glm::vec2 rotation)
{
    float pitch = glm::radians(rotation.x);
    float yaw = glm::radians(rotation.y);

    m_Direction.x = std::cos(yaw) * std::cos(pitch);
    m_Direction.y = std::sin(pitch);
    m_Direction.z = std::sin(yaw) * std::cos(pitch);
    m_Direction.w = 0.0f;
}
