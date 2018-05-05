#pragma once

#include "Scene.hpp"

#include "Components/SceneCamera.hpp"

namespace EVA
{

	class SceneEditor : public Scene
	{

		std::shared_ptr<GameObject> m_Camera;

	public:

		explicit SceneEditor(const std::string& path) : Scene(path)
		{
			// Camera
			m_Camera = CreateGameObject();
			m_Camera->SetName("SceneCamera");
			const auto cameraComponent = m_Camera->AddComponent<Camera>();
			m_Camera->AddComponent<SceneCamera>();
			Application::SetMainCamera(cameraComponent);
		}

		void Update(const float deltaTime) override
		{
			m_Camera->Update(deltaTime);
			m_Camera->LateUpdate();
			//Scene::Update(deltaTime);
		}
	};

}
