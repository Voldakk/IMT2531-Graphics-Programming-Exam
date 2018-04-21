#include "Component.hpp"

#include "Scene.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

namespace EVA
{

    Component::Component(GameObject *gameObject) : m_GameObject(gameObject)
    {
		if (gameObject != nullptr)
		{
			m_Scene = gameObject->scene.Get();

			if (gameObject->transform != nullptr)
			{
				m_Transform = gameObject->transform.get();
			}
		}
    }
}