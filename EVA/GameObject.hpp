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
		
	    /**
	     * \brief Constructor. Adds a transform to the game object
	     * \param scene The scene the game object is located in
	     */
	    explicit GameObject(Scene* scene);

		/**
		* \brief Runs every frame
		* \param deltaTime The time in seconds between frames
		*/
        void Update(float deltaTime);

	    /**
         * \brief Adds a component of the given type to the game object
         * \tparam T The component type
         * \tparam Args Any arguments passed to the components constructor
         * \param args Any arguments passed to the components constructor
         * \return A pointer to the component
         */
        template<class T, typename... Args>
        T* AddComponent(Args... args);

	    /**
         * \brief Sets the parent of the gameobject's transform
         * \param newParent The new parent's game object
         */
        void SetParent(GameObject *newParent) const;

	    /**
         * \brief Sets the parent of the gameobject's transform
         * \param newParent The new parent's game object
         */
        void SetParent(std::shared_ptr<GameObject> &newParent) const;

	    /**
		 * \brief Destroys the game object
		 */
		void Destroy();
    };

    template<class T, typename... Args>
	T* GameObject::AddComponent(Args... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(this, args...);
        m_Components.push_back(component);

        return component.get();
    }
}