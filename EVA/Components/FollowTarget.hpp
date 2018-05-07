#pragma once

#include "EVA.hpp"

namespace EVA
{

	class FollowTarget : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(FollowTarget)

	public:

		Transform * target = nullptr;
		glm::vec3 offset = glm::vec3(0.0f);
		float scalar = 1.0f;

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
	};
}