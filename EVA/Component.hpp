#pragma once

#include "ConstPointer.hpp"

#include "Parsers/Json.hpp"

namespace EVA
{

	class Scene;
	class GameObject;
	class Transform;

	/**
	* \brief Used to manage the active state a Component
	*/
	class IActive
	{
		bool m_Active = true;

	public:

		const bool& activeInternal = m_Active;
		void SetActiveInternal(bool value);
	};


	/**
	* \brief Gives a Component access to the Update method
	*/
	class IUpdateComponent : public IActive
	{

	public:

		/**
		* \brief Runs every frame
		* \param deltaTime The time in seconds between frames
		*/
		virtual void Update(float deltaTime) = 0;
	};

	/**
	* \brief Gives a Component access to the LateUpdate method
	*/
	class ILateUpdateComponent : public IActive
	{

	public:
		/**
		* \brief Called after Update
		*/
		virtual void LateUpdate() = 0;
	};

	/**
	* \brief Gives a Component access to the Render method
	*/
	class IRenderComponent : public IActive
	{

	public:
		/**
		* \brief Used to render game elements to the screen
		*/
		virtual void Render() = 0;
	};

	/**
	* \brief Base class for components
	*/
    class Component
    {
		friend GameObject;

		Scene* m_Scene;
		GameObject* m_GameObject;
		Transform* m_Transform;

		bool m_Active = true;

    public:

		const ConstPointer<Scene> scene = &m_Scene;
		const ConstPointer<GameObject> gameObject = &m_GameObject;
		const ConstPointer<Transform> transform = &m_Transform;

		const bool& active = m_Active;
	    
	    /**
	     * \brief Default constructor
	     */
		Component() = default;

		/**
		* \brief Default destructor
		*/
		virtual ~Component() = default;

		/**
		* \brief Gets the refferences to the scene and the transform on the game object
		* \param gameObject The GameObject the component is attaced to
		*/
		void SetGameObject(GameObject* gameObject);

	    /**
		 * \brief Sets the scene
		 * \param scene The new scene
		 */
		void SetScene(Scene* scene);
		
	    /**
    	 * \brief Sets the component's active state
    	 * \param value The new state
    	 */
    	void SetActive(bool value);

		/**
		* \brief Gets a component of the given type form the game object
		* \tparam T The type of component
		* \return A pointer to the component, or nullptr
		*/
		template<class T>
		T* GetComponentOfType();

		/**
		* \brief Called before the scene starts
		*/
		virtual void Awake();

		/**
		* \brief Called when the scene starts
		*/
		virtual void Start();

	    /**
		 * \brief Loads component values from the given data
		 * \param data The data
		 */
		virtual void Load(const DataObject data);
    };

	template <class T>
	T* Component::GetComponentOfType()
	{
		return m_GameObject->GetComponentOfType<T>();
	}
}
