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

	void GameObject::Awake()
	{
		for (auto &component : m_Components)
		{
			component->Awake();
		}
	}

	void GameObject::Start()
	{
		for (auto &component : m_Components)
		{
			component->Start();
		}
	}

	void GameObject::Update(const float deltaTime)
    {
        for (auto &component : m_UpdateComponents)
        {
			if(component->activeInternal)
				component->Update(deltaTime);
        }
    }

	void GameObject::LateUpdate()
	{
		for (auto &component : m_LateUpdateComponents)
		{
			if (component->activeInternal)
				component->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		for (auto &component : m_RenderComponents)
		{
			if (component->activeInternal)
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

	void GameObject::RemoveComponent(Component* component)
	{
		for (unsigned int i = 0; i < m_Components.size(); ++i)
		{
			if (m_Components[i].get() == component)
			{
				m_Components.erase(m_Components.begin() + i);
				break;
			}
		}

		// Update
		const auto uc = dynamic_cast<IUpdateComponent*>(component);
		if (uc != nullptr)
		{
			for (unsigned int i = 0; i < m_UpdateComponents.size(); ++i)
			{
				if (m_UpdateComponents[i] == uc)
				{
					m_UpdateComponents.erase(m_UpdateComponents.begin() + i);
					break;
				}
			}
		}

		// Late update
		const auto luc = dynamic_cast<ILateUpdateComponent*>(component);
		if (luc != nullptr)
		{
			for (unsigned int i = 0; i < m_LateUpdateComponents.size(); ++i)
			{
				if (m_LateUpdateComponents[i] == luc)
				{
					m_LateUpdateComponents.erase(m_LateUpdateComponents.begin() + i);
					break;
				}
			}
		}

		// Render
		const auto rc = dynamic_cast<IRenderComponent*>(component);
		if (rc != nullptr)
		{
			for (unsigned int i = 0; i < m_RenderComponents.size(); ++i)
			{
				if (m_RenderComponents[i] == rc)
				{
					m_RenderComponents.erase(m_RenderComponents.begin() + i);
					break;
				}
			}
		}
	}

	void GameObject::SetParent(GameObject *newParent) const
    {
        m_Transform->SetParent(newParent->m_Transform.get());
    }

	void GameObject::Destroy()
	{
		scene->DestroyGameObject(this);
	}

	void GameObject::SetName(const std::string& newName)
	{
		if (!m_Name.empty())
			scene->RemoveFromNameMap(this);

		m_Name = newName;

		const auto name = scene->AddToNameMap(this);
		m_Name = name;
	}

	std::string GameObject::GetName() const
	{
		return m_Name;
	}

	std::vector<std::shared_ptr<Component>>& GameObject::GetComponents()
	{
		return m_Components;
	}

	void GameObject::SetActive(const bool value)
	{
		m_Active = value;

		for (auto& component : m_Components)
		{
			component->SetActive(value);
		}
	}
}
