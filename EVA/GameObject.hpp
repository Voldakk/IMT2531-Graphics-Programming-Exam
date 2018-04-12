#pragma once

#include <vector>
#include <memory>

#include "ConstPointer.hpp"
#include "Component.hpp"
#include "Transform.hpp"

namespace EVA
{

    class Scene;

    class GameObject
    {

		Scene* m_Scene;
        std::shared_ptr<Transform> m_Transform;

        std::vector<std::shared_ptr<Component>> m_Components;

    public:

		// Public read only fields
		const ConstPointer<Scene> scene = &m_Scene;
		const std::shared_ptr<Transform>& transform = m_Transform;
		
	    explicit GameObject(Scene* scene);

        void Update(float deltaTime);

        template<class T, typename... Args>
        std::shared_ptr<T> AddComponent(Args... args);

        void SetParent(GameObject *newParent) const;

        void SetParent(std::shared_ptr<GameObject> &newParent) const;
    };

    template<class T, typename... Args>
    std::shared_ptr<T> GameObject::AddComponent(Args... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(this, args...);
        m_Components.push_back(component);

        return component;
    }
}