#include "Component.hpp"

#include "GameObject.hpp"
#include "Transform.hpp"

namespace EVA
{

    Component::Component(GameObject *gameObject) : m_GameObject(gameObject)
    {
		if(gameObject != nullptr && gameObject->transform != nullptr)
			m_Transform = gameObject->transform.get();
    }

    void Component::Update(float deltaTime)
    {

    }

}