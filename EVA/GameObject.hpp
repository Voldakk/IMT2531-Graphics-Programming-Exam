#pragma once

#include <vector>
#include <memory>

#include "ConstPointer.hpp"
#include "Component.hpp"
#include "Components/Transform.hpp"

namespace EVA
{

    class Scene;

	/**
     * \brief A game object. Holds different components to build more complex behaviours
     */
    class GameObject
    {

		Scene* m_Scene;
        std::unique_ptr<Transform> m_Transform;

		std::vector<std::shared_ptr<Component>> m_Components;
		std::vector<IUpdateComponent*> m_UpdateComponents;
		std::vector<ILateUpdateComponent*> m_LateUpdateComponents;
		std::vector<IRenderComponent*> m_RenderComponents;

		std::string m_Name;

		bool m_Active = true;

    public:

		// Public read only fields
		const bool& active = m_Active;
		const ConstPointer<Scene> scene = &m_Scene;
		const std::unique_ptr<Transform>& transform = m_Transform;
		
	    /**
	     * \brief Constructor. Adds a transform to the game object
	     * \param scene The scene the game object is located in
	     */
	    explicit GameObject(Scene* scene);

		/**
		* \brief Called before the scene starts
		*/
		void Awake();

		/**
		* \brief Called when the scene starts
		*/
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

	    /**
		 * \brief Attaches an existing component to the game object
		 * \param component The component
		 * \return A pointer to the component
		 */
		Component* AttachComponent(const std::shared_ptr<Component>& component);

		void RemoveComponent(Component* component);

		/**
		* \brief Gets a component of the given type from the game object
		* \tparam T The type of component
		* \return A pointer to the component, or nullptr
		*/
		template<class T>
		T* GetComponentOfType();

		/**
		* \brief Gets the components of the given type from the game object
		* \tparam T The type of component
		* \return A list of pointers to the components
		*/
		template<class T>
		std::vector<T*> GetComponentsOfType();

	    /**
         * \brief Sets the parent of the gameobject's transform
         * \param newParent The new parent's game object
         */
        void SetParent(GameObject *newParent) const;

	    /**
		 * \brief Destroys the game object
		 */
		void Destroy();

	    /**
		 * \brief Sets the name of the game object
		 * \param newName The new name
		 */
		void SetName(const std::string& newName);

	    /**
		 * \brief Gets the name of the game object
		 * \return The name
		 */
		std::string GetName() const;

		std::vector<std::shared_ptr<Component>>& GetComponents();

		void SetActive(bool value);
    };

    template<class T>
	T* GameObject::AddComponent()
    {
        std::shared_ptr<T> component = std::make_shared<T>();
        
		AttachComponent(component);

        return component.get();
    }

	template <class T>
	T* GameObject::GetComponentOfType()
	{
		for (auto component : m_Components)
		{
			T* pointer = dynamic_cast<T*>(component.get());
			if (pointer != nullptr)
				return pointer;
		}

		return nullptr;
	}
	template<class T>
	inline std::vector<T*> GameObject::GetComponentsOfType()
	{
		std::vector<T*> components;

		for (auto component : m_Components)
		{
			T* pointer = dynamic_cast<T*>(component.get());
			if (pointer != nullptr)
				components.push_back(pointer);
		}

		return components;
	}
}
