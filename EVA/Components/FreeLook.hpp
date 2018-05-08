#pragma once

#include "EVA.hpp"

#include "Camera.hpp"


namespace EVA
{

	class FreeLook : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(FreeLook)

		Camera* m_Camera = nullptr;

	public:

		bool wasd = true;

		float mouseSensitivity = 50.0f;
		float movementSpeed = 10.0f;

		float pitch = 0.0f;
		float yaw = 0.0f;
		
		void Start() override;

		void Update(const float deltaTime) override;

		/**
		* \brief Loads Component values from the given DataObject
		* \param data The DataObject
		*/
		void Load(DataObject data) override;

		/**
		* \brief Saves Component values to the given DataObject
		* \param data The DataObject
		*/
		void Save(DataObject& data) override;

		/**
		* \brief Draws the component inspector
		*/
		void Inspector() override;
	};
}