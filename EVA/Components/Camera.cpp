#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "../GameObject.hpp"
#include "../Application.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Camera, "EVA::Camera")

	Camera::~Camera()
	{
		if (Application::mainCamera == this)
			Application::UseDefaultCamera();
	}

	void Camera::Start()
	{
		EVA::Application::SetMainCamera(this);
	}

	void Camera::LateUpdate()
    {
    	// Update view
		m_ViewMatrix = glm::lookAt(transform->position, transform->position + transform->forward, transform->up);
    }

	void Camera::Load(const DataObject data)
	{
		fov = data.GetFloat("fov", 90.0f);
		near = data.GetFloat("near", 0.1f);
		far = data.GetFloat("far", 1000.0f);
	}
}
