#include "GameObject.hpp"

#include "Scene.hpp"

namespace EVA
{
    GameObject::GameObject(Scene* scene): m_Scene(scene)
	{
	    m_Transform = std::make_shared<Transform>(this);
		m_Transform->m_Transform = m_Transform.get();
    }

	void GameObject::Update(const float deltaTime)
    {
        for (auto &component : m_UpdateComponents)
        {
            component->Update(deltaTime);
        }
    }

	void GameObject::LateUpdate()
	{
		for (auto &component : m_LateUpdateComponents)
		{
			component->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		for (auto &component : m_RenderComponents)
		{
			component->Render();
		}
	}

	void GameObject::SetParent(GameObject *newParent) const
    {
        m_Transform->SetParent(newParent->m_Transform);
    }

    void GameObject::SetParent(std::shared_ptr<GameObject> &newParent) const
    {
        m_Transform->SetParent(newParent->m_Transform);
    }

	void GameObject::Destroy()
	{
		scene->DestroyGameObject(this);
	}
}
