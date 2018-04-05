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

		std::vector<std::vector<std::vector<MeshRenderer *>>> m_MeshRenderers;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
		std::vector<std::shared_ptr<Light>> m_Lights;

	public:

		/// <summary>Shared pointer to itself</summary>
		std::shared_ptr<Scene> self;

		/// <summary></summary>
		std::unique_ptr<Skybox> skybox;

		Scene();

		virtual ~Scene() = default;

		/// <summary>Allows the user to close the menu by pressing escape</summary>
		/// <param name="deltaTime">The time in seconds between frames</param>
		virtual void Update(float deltaTime);

		/// <summary>Render all scene elements to the screen</summary>
		virtual void Render();

		/// <summary></summary>
		std::shared_ptr<GameObject> CreateGameObject();

		/// <summary></summary>
		std::shared_ptr<Light> CreateLight(LightType type, const bool shadows = false, const unsigned int shadowSize = 4096);

		/// <summary></summary>
		void RegisterMeshRenderer(MeshRenderer *meshRenderer);

        /// <summary></summary>
		std::vector<std::shared_ptr<Light>> GetLights() { return m_Lights; }

	private:

		void RenderScene(glm::mat4 lightSpaceMatrix);
		void RenderShadowMap(glm::mat4 lightSpaceMatrix);
	};

}