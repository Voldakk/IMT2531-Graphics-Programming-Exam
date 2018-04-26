#pragma once

#include "glm/glm.hpp"

#include "Component.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component, public ILateUpdateComponent
    {
		glm::mat4 m_ViewMatrix;

    public:

        float fov;
        float near;
        float far;

		const glm::mat4& viewMatrix = m_ViewMatrix;

        explicit Camera(GameObject *gameObject);
		~Camera();

        void LateUpdate() override;
    };

}
