#pragma once

#include "EVA.hpp"
#include "../Input.hpp"

namespace EVA
{

	class FreeLook : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(FreeLook)

		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;

	public:

		bool wasd = true;

		float mouseSensitivity = 50.0f;
		float movementSpeed = 5.0f;

		FreeLook() = default;

		void Update(const float deltaTime) override;

		void Load(const DataObject data) override;
	};
}