#include "Component.hpp"

#include "Scene.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

namespace EVA
{
	void IActive::SetActiveInternal(const bool value)
	{
		m_Active = value;
	}

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

	void Component::SetActive(const bool value)
	{
		m_Active = value;

		// Update
		const auto uc = dynamic_cast<IUpdateComponent*>(this);
		if (uc != nullptr)
			uc->SetActiveInternal(value);

		// LateUpdate
		const auto luc = dynamic_cast<ILateUpdateComponent*>(this);
		if (luc != nullptr)
			luc->SetActiveInternal(value);

		// Render
		const auto rc = dynamic_cast<IRenderComponent*>(this);
		if (rc != nullptr)
			rc->SetActiveInternal(value);
	}
}
