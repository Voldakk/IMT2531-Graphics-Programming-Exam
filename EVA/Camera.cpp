#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "GameObject.hpp"

namespace EVA
{

    Camera::Camera(GameObject *gameObject) : Component(gameObject)
    {
        fov = 90.0f;
        near = 0.1f;
        far = 1000.0f;

        mouseSensitivity = 50.0f;
        movementSpeed = 5.0f;

        UpdateDirections();
    }

    void Camera::Update(const float deltaTime)
    {
        // Movement
        auto position = m_GameObject->transform->localPosition;

        if (Input::Key(GLFW_KEY_W)) // Front
            position += m_Front * movementSpeed * deltaTime;
        if (Input::Key(GLFW_KEY_S))    // Back
            position -= m_Front * movementSpeed * deltaTime;

        if (Input::Key(GLFW_KEY_D))    // Right
            position += m_Right * movementSpeed * deltaTime;
        if (Input::Key(GLFW_KEY_A))    // Left
            position -= m_Right * movementSpeed * deltaTime;

        if (Input::Key(GLFW_KEY_SPACE)) // Up
            position += m_Up * movementSpeed * deltaTime;
        if (Input::Key(GLFW_KEY_LEFT_SHIFT)) // Down
            position -= m_Up * movementSpeed * deltaTime;

        m_GameObject->transform->SetPosition(position);

        // Look
        const auto mouseMovement = Input::MouseMovement();
		m_Pitch += -mouseMovement.y * mouseSensitivity * deltaTime;
		m_Yaw += mouseMovement.x * mouseSensitivity * deltaTime;

		// Clamp
		m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

		m_GameObject->transform->SetOrientation(YAXIS, m_Yaw);
		m_GameObject->transform->Rotate(XAXIS, m_Pitch);

        // Update view
        UpdateDirections();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    void Camera::UpdateDirections()
    {
        const auto position = m_GameObject->transform->position;

		const auto orientation = m_GameObject->transform->orientation;

		m_Front = glm::normalize(ZAXIS * orientation);
        m_Right = glm::normalize(-XAXIS * orientation);
        m_Up = glm::normalize(YAXIS * orientation);

        m_ViewMatrix = glm::lookAt(position, position + m_Front, m_Up);
    }
}