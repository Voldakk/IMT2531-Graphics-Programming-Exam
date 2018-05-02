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

	void GameObject::SetParent(GameObject *newParent) const
    {
        m_Transform->SetParent(newParent->m_Transform.get());
    }

	void GameObject::Destroy()
	{
		scene->DestroyGameObject(this);
	}
}
