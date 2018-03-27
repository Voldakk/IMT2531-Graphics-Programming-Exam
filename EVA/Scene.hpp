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

	private:

		std::vector<std::vector<std::vector<MeshRenderer *>>> m_MeshRenderers;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects;

	public:

		/// <summary>Shared pointer to itself</summary>
		std::shared_ptr<Scene> self;

		/// <summary></summary>
		Light light;

		/// <summary></summary>
		std::unique_ptr<Skybox> skybox;

		/// <summary>Allows the user to close the menu by pressing escape</summary>
		/// <param name="deltaTime">The time in seconds between frames</param>
		virtual void Update(float deltaTime);

		/// <summary>Render all scene elements to the screen</summary>
		virtual void Render();

		/// <summary></summary>
		std::shared_ptr<GameObject> CreateGameObject();

		/// <summary></summary>
		void RegisterMeshRenderer(MeshRenderer *meshRenderer);
	};

}