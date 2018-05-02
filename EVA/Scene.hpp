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

	// A scene that can be run by the SceneManager
	class Scene
	{

		std::vector<std::vector<std::vector<MeshRenderer *>>> m_Materials;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
		std::vector<std::shared_ptr<Light>> m_Lights;

		std::vector<GameObject*> m_DestroyQueue;

		std::vector<std::shared_ptr<UiElement>> m_UiElements;

		bool m_Abort = false;
		

	public:
		const bool& abort = m_Abort;
		std::shared_ptr<Scene> self;
		inline void Abort()
		{
			m_Abort = true;
		}


		/// <summary></summary>
		std::unique_ptr<Skybox> skybox;

		Scene();

		explicit Scene(std::string path);

		~Scene();

		
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
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<GameObject> CreateGameObject();

		/**
		* \brief
		* \param gameObject
		*/
		void DestroyGameObject(GameObject * gameObject);

		/**
		 * \brief 
		 * \param type 
		 * \param shadows 
		 * \param shadowSize 
		 * \return 
		 */
		std::shared_ptr<Light> CreateLight(LightType type, bool shadows = false, unsigned int shadowSize = Light::DEFAULT_SHADOW_MAP_SIZE);

		Light* CreateLight(DataObject data);

		/**
		* \brief
		* \param meshRenderer
		*/
		void RegisterMeshRenderer(MeshRenderer *meshRenderer);

		/**
		* \brief
		* \param removeMeshRenderer
		*/
		void RemoveMeshRenderer(MeshRenderer *removeMeshRenderer);

		/**
		 * \brief 
		 * \return 
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

		void ProcessDestroyQueue();
	};

	template <class T, typename... Args>
	T* Scene::CreateUiElement(Args... args)
	{
		auto element = std::make_shared<T>(args...);
		m_UiElements.push_back(element);
		return element.get();
	}
}
