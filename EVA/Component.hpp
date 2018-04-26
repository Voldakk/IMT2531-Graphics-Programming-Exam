#pragma once

#include "ConstPointer.hpp"

namespace EVA
{

	class Scene;
	class GameObject;
	class Transform;

    class Component
    {
		friend GameObject;

		Scene* m_Scene;
		GameObject* m_GameObject;
		Transform* m_Transform;

    public:

		const ConstPointer<Scene> scene = &m_Scene;
		const ConstPointer<GameObject> gameObject = &m_GameObject;
		const ConstPointer<Transform> transform = &m_Transform;
	    
	    /**
	     * \brief Constructor. gets the refferences to the scene and the transform on the game object
	     * \param gameObject The GameObject the component is attaced to
	     */
	    explicit Component(GameObject *gameObject);

	    /**
		 * \brief Destructor
		 */
		virtual ~Component() = default;
    };

	class IUpdateComponent
	{

	public:

		/**
		* \brief Runs every frame
		* \param deltaTime The time in seconds between frames
		*/
		virtual void Update(float deltaTime) = 0;
	};

	class ILateUpdateComponent
	{

	public:
		/**
		* \brief Called after Update
		*/
		virtual void LateUpdate() = 0;
	};

	class IRenderComponent
	{

	public:
		/**
		* \brief Used to render game elements to the screen
		*/
		virtual void Render() = 0;
	};

}
