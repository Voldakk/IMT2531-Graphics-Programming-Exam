#include "FreeLook.hpp"

#include "EVA/Input.hpp"
#include "Transformer.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(FreeLook, "EVA::FreeLook")

		void FreeLook::Start()
	{
		m_Camera = gameObject->GetComponentOfType<Camera>();
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
		m_Pitch -= mouseMovement.y * mouseSensitivity * deltaTime;
		m_Yaw += mouseMovement.x * mouseSensitivity * deltaTime;

		// Clamp
		m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
		if (m_Yaw < 0.0f)
			m_Yaw += 360.0f;
		else if (m_Yaw > 360.0f)
			m_Yaw -= 360.0f;

		transform->SetOrientation(m_Pitch, m_Yaw, 0.0f);

		if (m_Camera != nullptr)
			m_Camera->fov -= Input::GetScroll().y;

		if (Input::KeyDown(Input::Alpha1))
		{
			auto target = scene->FindGameObjectByName("TargetCube");
			if (target != nullptr)
				target->transform->SetScale(2.0f);
		}
		if (Input::KeyDown(Input::Alpha2))
		{
			auto target = scene->FindComponentOfType<Transformer>();
			if (target != nullptr)
				target->SetActive(!target->active);
		}
	}

	void FreeLook::Load(const DataObject data)
	{
		wasd = data.GetBool("wasd", true);
		mouseSensitivity = data.GetFloat("mouseSensitivity", 50.0f);
		movementSpeed = data.GetFloat("movementSpeed", 5.0f);
	}
}
