#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "GameObject.hpp"
#include "Application.hpp"
#include "glm/gtx/quaternion.hpp"

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

	Camera::~Camera()
	{
		if (Application::mainCamera == this)
			Application::UseDefaultCamera();
	}

	void Camera::Update(const float deltaTime)
    {

        // Movement
		glm::vec3 movement;

		// Front
        if (Input::Key(Input::W))
        {
			movement += transform->forward;
        }

        // Back
        if (Input::Key(Input::S))
        {
			movement -= transform->forward;
        }


		// Right
        if (Input::Key(Input::D))
        {
			movement += transform->right;
        }

		// Left
        if (Input::Key(Input::A))
		{
			movement -= transform->right;
		}


		// Up
		if (Input::Key(Input::Space))
		{
			movement += transform->up;
		}

		// Down
		if (Input::Key(Input::LeftShift))
		{
			movement -= transform->up;
		}

		transform->Translate(movement * movementSpeed * deltaTime);

        // Look
        const auto mouseMovement = Input::MouseMovement();
		m_Pitch -= mouseMovement.y * mouseSensitivity * deltaTime;
		m_Yaw -= mouseMovement.x * mouseSensitivity * deltaTime;

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
		auto o = transform->orientation;
		o.y *= -1;
		o.z *= -1;
	    const auto forward = glm::normalize(ZAXIS * o);
		const auto up = glm::normalize(YAXIS * o);

		m_ViewMatrix = glm::lookAt(transform->position, transform->position + forward, up);
    }
}
