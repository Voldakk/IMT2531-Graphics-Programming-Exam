#pragma once

#include "glm/glm.hpp"

#include "Component.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component, public IUpdateComponent
    {
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;

		glm::mat4 m_ViewMatrix;

    public:

        float fov;
        float near;
        float far;

        float mouseSensitivity;
        float movementSpeed;

        explicit Camera(GameObject *gameObject);

        void Update(float deltaTime) override;

        glm::mat4 GetViewMatrix() const;

    private:

        void UpdateDirections();
    };

}
