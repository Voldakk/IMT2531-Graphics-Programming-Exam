#include "FreeLook.hpp"

#include "EVA/Input.hpp"
#include "Transformer.hpp"
#include "Transform.hpp"

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
		if (Input::Key(keyBindings[Forward]))
			currentMovementSpeed *= 1.0 + deltaTime;
		if (Input::KeyUp(keyBindings[Forward]))
			currentMovementSpeed = minMovementSpeed;

		// Movement
		glm::vec3 movement;

		// Front
		if (Input::Key(keyBindings[Forward]))
			movement += transform->forward;

		// Back
		if (Input::Key(keyBindings[Back]))
			movement -= transform->forward;

		// Right
		if (Input::Key(keyBindings[Right]))
			movement += transform->right;

		// Left
		if (Input::Key(keyBindings[Left]))
			movement -= transform->right;

		// Up
		if (Input::Key(keyBindings[Up]))
			movement += transform->up;

		// Down
		if (Input::Key(keyBindings[Down]))
			movement -= transform->up;

		transform->Translate(movement * currentMovementSpeed * deltaTime);

		// Look
		const auto mouseMovement = Input::MouseMovement();
		pitch -= mouseMovement.y * mouseSensitivity * deltaTime;
		yaw -= mouseMovement.x * mouseSensitivity * deltaTime;

		// Clamp
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		if (yaw < 0.0f)
			yaw += 360.0f;
		else if (yaw > 360.0f)
			yaw -= 360.0f;

		transform->SetOrientation(YAXIS, yaw);
		transform->Rotate(transform->right, pitch);

		if (m_Camera != nullptr)
			m_Camera->fov -= Input::GetScroll().y;
	}

	void FreeLook::Load(const DataObject data)
	{
		mouseSensitivity = data.GetFloat("mouseSensitivity", mouseSensitivity);
		minMovementSpeed = data.GetFloat("movementSpeed", minMovementSpeed);
		currentMovementSpeed = minMovementSpeed;

		pitch = data.GetFloat("pitch", 0.0f);
		yaw = data.GetFloat("yaw", 0.0f);
	}

	void FreeLook::Save(DataObject& data)
	{
		data.SetFloat("mouseSensitivity", mouseSensitivity);
		data.SetFloat("movementSpeed", minMovementSpeed);

		data.SetFloat("pitch", pitch);
		data.SetFloat("yaw", yaw);
	}

	void FreeLook::Inspector()
	{
		ComponentInspector::Float("Mouse sensitivity", mouseSensitivity);
		ComponentInspector::Float("Movement speed", minMovementSpeed);
	}
}
