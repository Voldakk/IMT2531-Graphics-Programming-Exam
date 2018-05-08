#include "FreeLook.hpp"

#include "EVA/Input.hpp"
#include "Transformer.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(FreeLook, "EVA::FreeLook")

	void FreeLook::Start()
	{
		m_Camera = gameObject->GetComponentOfType<Camera>();
		Input::SetCursorMode(Input::Disabled);
	}

	void FreeLook::Update(const float deltaTime)
	{
		// Movement
		glm::vec3 movement;

		// Front
		if (Input::Key(wasd ? Input::W : Input::Up))
			movement += transform->forward;

		// Back
		if (Input::Key(wasd ? Input::S : Input::Down))
			movement -= transform->forward;

		// Right
		if (Input::Key(wasd ? Input::D : Input::Right))
			movement += transform->right;

		// Left
		if (Input::Key(wasd ? Input::A : Input::Left))
			movement -= transform->right;

		// Up
		if (Input::Key(wasd ? Input::Space : Input::RightShift))
			movement += transform->up;

		// Down
		if (Input::Key(wasd ? Input::LeftShift : Input::RightControl))
			movement -= transform->up;

		transform->Translate(movement * movementSpeed * deltaTime);

		// Look
		const auto mouseMovement = Input::MouseMovement();
		pitch -= mouseMovement.y * mouseSensitivity * deltaTime;
		yaw += mouseMovement.x * mouseSensitivity * deltaTime;

		// Clamp
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		if (yaw < 0.0f)
			yaw += 360.0f;
		else if (yaw > 360.0f)
			yaw -= 360.0f;

		transform->SetOrientation(pitch, yaw, 0.0f);

		if (m_Camera != nullptr)
			m_Camera->fov -= Input::GetScroll().y;
	}

	void FreeLook::Load(const DataObject data)
	{
		wasd = data.GetBool("wasd", true);
		mouseSensitivity = data.GetFloat("mouseSensitivity", 50.0f);
		movementSpeed = data.GetFloat("movementSpeed", 10.0f);

		pitch = data.GetFloat("pitch", 0.0f);
		yaw = data.GetFloat("yaw", 0.0f);
	}

	void FreeLook::Save(DataObject& data)
	{
		data.SetBool("wasd", wasd);
		data.SetFloat("mouseSensitivity", mouseSensitivity);
		data.SetFloat("movementSpeed", movementSpeed);

		data.SetFloat("pitch", pitch);
		data.SetFloat("yaw", yaw);
	}

	void FreeLook::Inspector()
	{
		ComponentInspector::Bool("WASD", wasd);
		ComponentInspector::Float("Mouse sensitivity", mouseSensitivity);
		ComponentInspector::Float("Movement speed", movementSpeed);
	}
}
