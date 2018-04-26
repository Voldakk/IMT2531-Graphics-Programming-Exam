#pragma once

#include "glm/glm.hpp"

#include "Component.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component, public IUpdateComponent
    {
		glm::mat4 m_ViewMatrix;

    public:

        float fov;
        float near;
        float far;

        explicit Camera(GameObject *gameObject);
		~Camera();

        void Update(float deltaTime) override;

        glm::mat4 GetViewMatrix() const;

    private:

        void UpdateDirections();
    };

}
