#pragma once

#include "glm/glm/glm.hpp"

#include "Component.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component
    {

    private:

        glm::mat4 m_ViewMatrix;

        glm::vec3 m_Front;
        glm::vec3 m_Right;
        glm::vec3 m_Up;

    public:

        float m_Fov;
        float m_Near;
        float m_Far;

        float m_MouseSensitivity;
        float m_MovementSpeed;

        explicit Camera(GameObject *gameObject);

        void Update(float deltaTime) override;

        glm::mat4 GetViewMatrix() const;

    private:

        void UpdateDirections();
    };

}