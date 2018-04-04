#pragma once

#include "glm/glm.hpp"

#include "Component.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component
    {

        glm::mat4 m_ViewMatrix;

        glm::vec3 m_Front;
        glm::vec3 m_Right;
        glm::vec3 m_Up;

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