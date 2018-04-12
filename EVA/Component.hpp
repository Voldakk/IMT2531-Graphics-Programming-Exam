#pragma once

#include "ConstPointer.hpp"

namespace EVA
{

	class Scene;
	class GameObject;
	class Transform;

    class Component
    {
		friend GameObject;

		Scene* m_Scene;
		GameObject* m_GameObject;
		Transform* m_Transform;

    public:

		const ConstPointer<Scene> scene = &m_Scene;
		const ConstPointer<GameObject> gameObject = &m_GameObject;
		const ConstPointer<Transform> transform = &m_Transform;
	    
	    explicit Component(GameObject *gameObject);

		virtual ~Component() = default;

        virtual void Update(float deltaTime);
    };

}
