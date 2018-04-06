#pragma once

#include <vector>
#include <memory>

#include "Component.hpp"
#include "Transform.hpp"

namespace EVA
{

    class Scene;

    class GameObject
    {

        Scene *m_Scene{};
        std::shared_ptr<Transform> m_Transform;
        std::vector<std::shared_ptr<Component>> m_Components;

    public:

	    explicit GameObject(Scene *scene);

        void Update(float deltaTime);

        template<class T, typename... Args>
        std::shared_ptr<T> AddComponent(Args... args);

        void SetParent(GameObject *newParent);

        void SetParent(std::shared_ptr<GameObject> &newParent);

        int GetChildIndex(Transform *child) const;

        int GetChildIndex(GameObject *child) const;

        inline Scene *GetScene() const
        { return m_Scene; }

        inline std::shared_ptr<Transform> GetTransform() const
        { return m_Transform; }

		inline void SetPosition(const glm::vec3 newPosition) const { m_Transform->SetPosition(newPosition); }
		inline void SetRotation(const glm::vec3 newRotation) const { m_Transform->SetRotation(newRotation); }
		inline void SetScale(const glm::vec3 newScale) const { m_Transform->SetScale(newScale); }
    };

    template<class T, typename... Args>
    std::shared_ptr<T> GameObject::AddComponent(Args... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(this, args...);
        m_Components.push_back(component);

        return component;
    }
}