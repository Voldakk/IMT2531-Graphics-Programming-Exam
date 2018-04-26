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

        UpdateDirections();
    }

	Camera::~Camera()
	{
		if (Application::mainCamera == this)
			Application::UseDefaultCamera();
	}

	void Camera::Update(const float deltaTime)
    {
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
