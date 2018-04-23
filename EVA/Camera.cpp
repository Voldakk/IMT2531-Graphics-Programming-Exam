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
        if (Input::Key(Input::W))
        {
			movement.x -= transform->forward.x;
			movement.y += transform->forward.y;
			movement.z += transform->forward.z;
        }

        // Back
        if (Input::Key(Input::S))
        {
			movement.x -= -transform->forward.x;
			movement.y += -transform->forward.y;
			movement.z += -transform->forward.z;
        }


		// Right
        if (Input::Key(Input::D))
        {
			movement.x += transform->right.x;
			movement.y += transform->right.y;
			movement.z -= transform->right.z;
        }

		// Left
        if (Input::Key(Input::A))
		{
			movement.x += -transform->right.x;
			movement.y += -transform->right.y;
			movement.z -= -transform->right.z;
		}


		// Up
		if (Input::Key(Input::Space))
		{
			movement.x -= transform->up.x;
			movement.y += transform->up.y;
			movement.z += transform->up.z;
		}

		// Down
		if (Input::Key(Input::LeftShift))
		{
			movement.x -= -transform->up.x;
			movement.y += -transform->up.y;
			movement.z += -transform->up.z;
		}

		transform->Translate(movement * movementSpeed * deltaTime);

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

		transform->SetOrientation(YAXIS, m_Yaw);
		transform->Rotate(XAXIS, m_Pitch);

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

	    const auto tp = transform->position;
	    const auto position = glm::vec3(-tp.x, tp.y, tp.z);

		m_ViewMatrix = glm::lookAt(position, position + transform->forward, transform->up);
    }
}
