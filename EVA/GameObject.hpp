#pragma once

#include <vector>
#include <memory>

#include "ConstPointer.hpp"
#include "Component.hpp"
#include "Components/Transform.hpp"

namespace EVA
{

    class Scene;

    class GameObject
    {

		Scene* m_Scene;
        std::unique_ptr<Transform> m_Transform;

		std::vector<std::shared_ptr<Component>> m_Components;
		std::vector<IUpdateComponent*> m_UpdateComponents;
		std::vector<ILateUpdateComponent*> m_LateUpdateComponents;
		std::vector<IRenderComponent*> m_RenderComponents;

    public:

		// Public read only fields
		const ConstPointer<Scene> scene = &m_Scene;
		const std::unique_ptr<Transform>& transform = m_Transform;
		
	    /**
	     * \brief Constructor. Adds a transform to the game object
	     * \param scene The scene the game object is located in
	     */
	    explicit GameObject(Scene* scene);

		void Start();

		/**
		* \brief Runs every frame
		* \param deltaTime The time in seconds between frames
		*/
		void Update(float deltaTime);

		/**
		* \brief Called after Update
		*/
		void LateUpdate();

		/**
		* \brief Used to render game elements to the screen
		*/
		void Render();

	    /**
         * \brief Adds a component of the given type to the game object
         * \tparam T The component type
         * \return A pointer to the component
         */
        template<class T>
        T* AddComponent();

		Component* AttachComponent(const std::shared_ptr<Component>& component);

	    /**
         * \brief Sets the parent of the gameobject's transform
         * \param newParent The new parent's game object
         */
        void SetParent(GameObject *newParent) const;

	    /**
		 * \brief Destroys the game object
		 */
		void Destroy();
    };

    template<class T>
	T* GameObject::AddComponent()
    {
        std::shared_ptr<T> component = std::make_shared<T>();
        
		AttachComponent(component);

        return component.get();
    }
}