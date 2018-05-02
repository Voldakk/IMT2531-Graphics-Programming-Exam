#include "GameObject.hpp"

#include "Scene.hpp"

namespace EVA
{
    GameObject::GameObject(Scene* scene): m_Scene(scene)
	{
	    m_Transform = std::make_unique<Transform>();
		m_Transform->SetGameObject(this);
		m_Transform->m_Transform = m_Transform.get();
    }

	void GameObject::Start()
	{
		for (auto &component : m_Components)
		{
			if (component->active)
				component->Start();
		}
	}

	void GameObject::Update(const float deltaTime)
    {
        for (auto &component : m_UpdateComponents)
        {
			if(component->active)
				component->Update(deltaTime);
        }
    }

	void GameObject::LateUpdate()
	{
		for (auto &component : m_LateUpdateComponents)
		{
			if (component->active)
				component->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		for (auto &component : m_RenderComponents)
		{
			if (component->active)
				component->Render();
		}
	}

	Component* GameObject::AttachComponent(const std::shared_ptr<Component>& component)
	{
		m_Components.push_back(component);

		component->SetGameObject(this);

		// Update
		const auto uc = dynamic_cast<IUpdateComponent*>(component.get());
		if (uc != nullptr)
			m_UpdateComponents.push_back(uc);

		// LateUpdate
		const auto luc = dynamic_cast<ILateUpdateComponent*>(component.get());
		if (luc != nullptr)
			m_LateUpdateComponents.push_back(luc);

		// Render
		const auto rc = dynamic_cast<IRenderComponent*>(component.get());
		if (rc != nullptr)
			m_RenderComponents.push_back(rc);

		return component.get();
	}

	void GameObject::SetParent(GameObject *newParent) const
    {
        m_Transform->SetParent(newParent->m_Transform.get());
    }

	void GameObject::Destroy()
	{
		scene->DestroyGameObject(this);
	}
}
