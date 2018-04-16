#pragma once

#include <vector>
#include <memory>

#include "Light.hpp"

#include "GameObject.hpp"
#include "Skybox.hpp"
#include "MeshRenderer.hpp"

namespace EVA
{

	// A scene that can be run by the SceneManager
	class Scene
	{

		std::vector<std::vector<std::vector<MeshRenderer *>>> m_Materials;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
		std::vector<std::shared_ptr<Light>> m_Lights;

		std::vector<GameObject*> m_DestroyQueue;

	public:

		/// <summary></summary>
		std::unique_ptr<Skybox> skybox;

		Scene();

		
		/**
		 * \brief Runs every frame
		 * \param deltaTime The time in seconds between frames
		 */
		virtual void Update(float deltaTime);

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
		std::shared_ptr<Light> CreateLight(LightType type, bool shadows = false, unsigned int shadowSize = 4096);

		/**
		* \brief
		* \param meshRenderer
		*/
		void RegisterMeshRenderer(MeshRenderer *meshRenderer);

		/**
		* \brief
		* \param meshRenderer
		*/
		void RemoveMeshRenderer(MeshRenderer *meshRenderer);

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<std::shared_ptr<Light>> GetLights() const { return m_Lights; }

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
	};

}