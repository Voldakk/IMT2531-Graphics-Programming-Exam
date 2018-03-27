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

    private:

        Scene *m_Scene{};
        std::shared_ptr<Transform> m_Transform;
        std::vector<std::shared_ptr<Component>> m_Components;

    public:

        GameObject(Scene *scene);

        void Update(float deltaTime);

        template<class T>
        std::shared_ptr<T> AddComponent();

        void SetParent(GameObject *newParent);

        void SetParent(std::shared_ptr<GameObject> &newParent);

        int GetChildIndex(Transform *child) const;

        int GetChildIndex(GameObject *child) const;

        inline Scene *GetScene() const
        { return m_Scene; }

        inline std::shared_ptr<Transform> GetTransform() const
        { return m_Transform; }
    };

    template<class T>
    std::shared_ptr<T> GameObject::AddComponent()
    {
        std::shared_ptr<T> component = std::make_shared<T>(this);
        m_Components.push_back(component);

        return component;
    }

}