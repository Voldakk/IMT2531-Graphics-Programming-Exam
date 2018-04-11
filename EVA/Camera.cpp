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
		glm::vec3 movement;

		// Front
        if (Input::Key(GLFW_KEY_W))
        {
			movement.x -= m_Transform->forward.x;
			movement.y += m_Transform->forward.y;
			movement.z += m_Transform->forward.z;
        }

        // Back
        if (Input::Key(GLFW_KEY_S))
        {
			movement.x -= -m_Transform->forward.x;
			movement.y += -m_Transform->forward.y;
			movement.z += -m_Transform->forward.z;
        }


		// Right
        if (Input::Key(GLFW_KEY_D))
        {
			movement.x += m_Transform->right.x;
			movement.y += m_Transform->right.y;
			movement.z -= m_Transform->right.z;
        }

		// Left
        if (Input::Key(GLFW_KEY_A))
		{
			movement.x += -m_Transform->right.x;
			movement.y += -m_Transform->right.y;
			movement.z -= -m_Transform->right.z;
		}


		// Up
		if (Input::Key(GLFW_KEY_SPACE))
		{
			movement.x -= m_Transform->up.x;
			movement.y += m_Transform->up.y;
			movement.z += m_Transform->up.z;
		}

		// Down
		if (Input::Key(GLFW_KEY_LEFT_SHIFT))
		{
			movement.x -= -m_Transform->up.x;
			movement.y += -m_Transform->up.y;
			movement.z += -m_Transform->up.z;
		}

		m_Transform->Translate(movement * movementSpeed * deltaTime);

        // Look
        const auto mouseMovement = Input::MouseMovement();
		m_Pitch += -mouseMovement.y * mouseSensitivity * deltaTime;
		m_Yaw += mouseMovement.x * mouseSensitivity * deltaTime;

		// Clamp
		m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
		if (m_Yaw < 0.0f)
			m_Yaw += 360.0f;
		else if (m_Yaw > 360.0f)
			m_Yaw -= 360.0f;

		m_Transform->SetOrientation(YAXIS, m_Yaw);
		m_Transform->Rotate(XAXIS, m_Pitch);

        // Update view
        UpdateDirections();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    void Camera::UpdateDirections()
    {
		// Orientation

	    const auto tp = m_Transform->position;
	    const auto position = glm::vec3(-tp.x, tp.y, tp.z);

		m_ViewMatrix = glm::lookAt(position, position + m_Transform->forward, m_Transform->up);
    }
}
