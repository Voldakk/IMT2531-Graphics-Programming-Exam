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

		GameObject* m_Selected = nullptr;		

	public:

		explicit SceneEditor(const std::string& path) : Scene(path)
		{
			// Camera
			m_SceneCameraGameObject = CreateGameObject();
			m_SceneCameraGameObject->SetName("EVA::EditorCamera");
			m_SceneCameraGameObject->AddComponent<Camera>();
			m_SceneCamera = m_SceneCameraGameObject->AddComponent<SceneCamera>();

			m_Selected = FindGameObjectByName("Ground plane");
		}

		void Update(const float deltaTime) override
		{
			m_SceneCameraGameObject->Update(deltaTime);

			if (Input::MouseButtonDown(Input::MouseLeft))
			{
				const auto mousePos = Input::MousePosition();
				const auto ray = Physics::ScreenPosToWorldRay(mousePos, Application::mainCamera);

				RaycastHit hit;
				if (Physics::Raycast(ray, hit, this))
				{
					std::cout << "Name: " << hit.hitCollider->gameObject->GetName() << ", Dist: " << hit.distance
						<< ", Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ") \n";

					m_Selected = hit.hitCollider->gameObject.Get();
				}
			}

			EditorWindows::SceneHierarchy(this, &m_Selected);

			EditorWindows::Inspector(m_Selected);

			ImGui::ShowDemoWindow();			
		}

		void LateUpdate() override
		{
			m_SceneCameraGameObject->LateUpdate();
		}
	};

}
