#pragma once

#include "Scene.hpp"

#include "Editor/EditorWindows.hpp"

#include "Components/SceneCamera.hpp"
#include "Parsers/SceneParser.hpp"
#include "glm/gtx/euler_angles.inl"

namespace EVA
{

	class SceneEditor : public Scene
	{

		std::shared_ptr<GameObject> m_SceneCameraGameObject;
		SceneCamera* m_SceneCamera;

		

	public:

		explicit SceneEditor(const std::string& path) : Scene(path)
		{
			// Camera
			m_SceneCameraGameObject = CreateGameObject();
			m_SceneCameraGameObject->SetName("EVA::EditorCamera");
			m_SceneCameraGameObject->AddComponent<Camera>();
			m_SceneCamera = m_SceneCameraGameObject->AddComponent<SceneCamera>();

			m_SceneCamera->selected = FindGameObjectByName("Ground plane");
		}

		void Update(const float deltaTime) override
		{
			m_SceneCameraGameObject->Update(deltaTime);

			EditorWindows::SceneHierarchy(this, &m_SceneCamera->selected);

			EditorWindows::Inspector(m_SceneCamera->selected);

			ImGui::ShowDemoWindow();
		}

		void LateUpdate() override
		{
			m_SceneCameraGameObject->LateUpdate();
		}
	};

}
