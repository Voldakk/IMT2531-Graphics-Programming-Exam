#pragma once

#include <vector>
#include <memory>

#include "Light.hpp"
#include "GameObject.hpp"
#include "Skybox.hpp"
#include "Components/MeshRenderer.hpp"
#include "UI/UiElement.hpp"

namespace EVA
{
	class Collider;

	/**
	 * \brief A scene that can be run by the SceneManager
	 */
	class Scene
	{
		bool m_Abort = false;

		ShadowMaterial m_ShadowMaterial;
		ShadowMaterial m_ShadowMaterialInstanced;

		ShadowMaterial m_ShadowMaterialCube;
		ShadowMaterial m_ShadowMaterialCubeInstanced;

	protected:
		
		bool m_Started = false;

		std::vector<std::vector<std::vector<MeshRenderer *>>> m_Materials;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
		std::vector<std::shared_ptr<Light>> m_Lights;

		std::vector<GameObject*> m_DestroyQueue;

		std::vector<Collider*> m_Colliders;

		std::vector<std::shared_ptr<UiElement>> m_UiElements;

		std::map<std::string, GameObject*> m_NameMap;
		unsigned int m_NameCounter = 0;

	public:

		const bool& started = m_Started;
		const bool& abort = m_Abort;
		std::shared_ptr<Scene> self;
		inline void Abort()
		{
			m_Abort = true;
		}


		/// <summary></summary>
		std::unique_ptr<Skybox> skybox;

		Scene();

		explicit Scene(const FS::path& path);

		~Scene();


		/**
		* \brief Called before the scene starts
		*/
		virtual void Awake();

		/**
		* \brief Called when the scene starts
		*/
		virtual void Start();

		/**
		 * \brief Runs every frame
		 * \param deltaTime The time in seconds between frames
		 */
		virtual void Update(float deltaTime);

		/**
		* \brief Called after Update
		*/
		virtual void LateUpdate();

		/**
		* \brief Render all scene elements to the screen
		*/
		virtual void Render();

		/**
		 * \brief Creates a new game object
		 * \return A pointer to the game object
		 */
		std::shared_ptr<GameObject> CreateGameObject();

		/**
		* \brief Destroys a game object
		* \param gameObject The game object
		*/
		void DestroyGameObject(GameObject * gameObject);

		/**
		 * \brief Adds a game object to the name map
		 * \param gameObject The game object
		 * \return The name that was added
		 */
		std::string AddToNameMap(GameObject* gameObject);

		/**
		 * \brief removes a game object from the name map
		 * \param gameObject The game object
		 */
		void RemoveFromNameMap(GameObject* gameObject);

		/**
		 * \brief Tries to find a game object with the given name
		 * \param name The name
		 * \return The game object, or nullptr
		 */
		GameObject* FindGameObjectByName(const std::string& name);

		/**
		* \brief Tries to find a component of the given type on any game object in the scene
		* \tparam T The type of component
		* \return A pointer to the component, or nullptr
		*/
		template<class T>
		T* FindComponentOfType();

		/**
		 * \brief Creates a new light in the scene
		 * \param type The type of light
		 * \param shadows Wheter the light has shadows
		 * \param shadowSize The size of the shadow map
		 * \return A pointer to the light
		 */
		std::shared_ptr<Light> CreateLight(Light::Type type, bool shadows = false, unsigned int shadowSize = Light::DEFAULT_SHADOW_MAP_SIZE);

		/**
		 * \brief Creates a new light in the scene from the given data
		 * \param data The data
		 * \return A pointer to the light
		 */
		Light* CreateLight(DataObject data);

		void DestroyLight(Light* light);

		/**
		* \brief Registers a mesh renderer to be rendered in the scene
		* \param meshRenderer The mesh renderer
		*/
		void RegisterMeshRenderer(MeshRenderer *meshRenderer);

		/**
		* \brief Removes a mesh renderer form the list mesh renderers in the scene
		* \param removeMeshRenderer The mesh renderer
		*/
		void RemoveMeshRenderer(MeshRenderer *removeMeshRenderer);

		/**
		 * \brief gets all the lights in the scene
		 * \return A list of lights
		 */
		std::vector<std::shared_ptr<Light>> GetLights() const { return m_Lights; }

		/**
		 * \brief Creates a new UiElement of type T
		 * \tparam T The type of UiElement that should be created
		 * \tparam Args Any agruments for the elements constructor
		 * \param args Any agruments for the elements constructor
		 * \return A pointer to the newly created element
		 */
		template<class T, typename... Args>
		T* CreateUiElement(Args... args);

		/**
		* \brief Calls OnScreenResize on all root UiElements
		 */
		void OnScreenResize();

		std::vector<std::shared_ptr<GameObject>>& GetGameObjects()
		{
			return m_GameObjects;
		}

		void AddCollider(Collider* collider);
		void RemoveCollider(Collider* collider);
		std::vector<Collider*>& GetColliders();

		void ProcessDestroyQueue();

	private:

		/**
		 * \brief 
		 */
		void RenderScene();

		/**
		 * \brief 
		 * \param lightSpaceMatrix 
		 */
		void RenderShadowMap(glm::mat4 lightSpaceMatrix);

		/**
		 * \brief 
		 * \param shadowMatrices 
		 * \param lightPos 
		 * \param farPlane 
		 */
		void RenderShadowCubeMap(const std::vector<glm::mat4>& shadowMatrices, glm::vec3 lightPos, float farPlane);

		void RenderUi();

	};

	template <class T>
	T* Scene::FindComponentOfType()
	{
		for (auto gameObject : m_GameObjects)
		{
			T* pointer = gameObject->GetComponentOfType<T>();
			if (pointer != nullptr)
				return pointer;
		}

		return nullptr;
	}

	template <class T, typename... Args>
	T* Scene::CreateUiElement(Args... args)
	{
		auto element = std::make_shared<T>(args...);
		m_UiElements.push_back(element);
		return element.get();
	}
}
