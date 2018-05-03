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

		if (!m_Name.empty())
			scene->AddToNameMap(this);
	}

	std::string GameObject::GetName() const
	{
		return m_Name;
	}
}
