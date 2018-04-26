#pragma once

#include "EVA.hpp"

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
			auto o = transform->orientation;
			o.y *= -1;
			o.z *= -1;
			const auto forward = glm::normalize(ZAXIS * o);
			const auto up = glm::normalize(YAXIS * o);
			const auto right = glm::normalize(-XAXIS * o);

			// Movement
			glm::vec3 movement;

			// Front
			if (Input::Key(Input::W))
			{
				movement += forward;
			}

			// Back
			if (Input::Key(Input::S))
			{
				movement -= forward;
			}


			// Right
			if (Input::Key(Input::D))
			{
				movement += right;
			}

			// Left
			if (Input::Key(Input::A))
			{
				movement -= right;
			}


			// Up
			if (Input::Key(Input::Space))
			{
				movement += up;
			}

			// Down
			if (Input::Key(Input::LeftShift))
			{
				movement -= up;
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
		}
	};

}