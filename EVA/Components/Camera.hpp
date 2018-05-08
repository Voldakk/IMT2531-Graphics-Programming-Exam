#pragma once

#include "glm/glm.hpp"

#include "../Component.hpp"
#include "../ComponentMap.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component, public ILateUpdateComponent
    {
		REGISTER_COMPONENT_HPP(Camera)

		glm::mat4 m_ViewMatrix;

    public:

        float fov = 90.0f;
        float near = 0.1f;
        float far = 1000.0f;

		const glm::mat4& viewMatrix = m_ViewMatrix;

        Camera() = default;
		~Camera();

		void Start() override;

        void LateUpdate() override;

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