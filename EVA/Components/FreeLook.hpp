#pragma once

#include "EVA.hpp"
#include "../Input.hpp"

namespace EVA
{

	class FreeLook : public Component, public IUpdateComponent
	{

		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;

	public:

		float mouseSensitivity = 50.0f;
		float movementSpeed = 5.0f;

		explicit FreeLook(GameObject* gameObject)
			: Component(gameObject)
		{

		}

		void Update(const float deltaTime) override
		{
			// Movement
			glm::vec3 movement;

			// Front
			if (Input::Key(Input::Up))
				movement += transform->forward;

			// Back
			if (Input::Key(Input::Down))
				movement -= transform->forward;

			// Right
			if (Input::Key(Input::Right))
				movement += transform->right;

			// Left
			if (Input::Key(Input::Left))
				movement -= transform->right;

			// Up
			if (Input::Key(Input::RightShift))
				movement += transform->up;

			// Down
			if (Input::Key(Input::RightControl))
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

			transform->SetOrientation(YAXIS, m_Yaw);
			transform->Rotate(XAXIS, m_Pitch);
		}
	};

}
