#pragma once

#include "Scene.hpp"

#include <iostream>
#include <memory>

#include "Physics.hpp"
#include "Components/SceneCamera.hpp"
#include "Editor/EditorWindows.hpp"

namespace EVA
{

	class SceneEditor : public Scene
	{

		std::shared_ptr<GameObject> m_SceneCameraGameObject;
		SceneCamera* m_SceneCamera;

		GameObject* m_Selected = nullptr;

		std::unique_ptr<EditorWindows> m_Ew;

	public:

		explicit SceneEditor(const std::string& path) : Scene(path)
		{
			// Camera
			m_SceneCameraGameObject = CreateGameObject();
			m_SceneCameraGameObject->SetName("EVA::EditorCamera");
			m_SceneCameraGameObject->AddComponent<Camera>();
			m_SceneCamera = m_SceneCameraGameObject->AddComponent<SceneCamera>();

			// Windows
			m_Ew = std::make_unique<EditorWindows>(this);
		}

		void Update(const float deltaTime) override
		{
			// Move camera
			m_SceneCameraGameObject->Update(deltaTime);

			// Check for clicked object
			if (Input::MouseButtonDown(Input::MouseLeft))
			{
				const auto mousePos = Input::MousePosition();
				const auto ray = Physics::ScreenPosToWorldRay(mousePos, Application::mainCamera);

				RaycastHit hit;
				if (Physics::Raycast(ray, hit, this))
				{
					std::cout << "Name: " << hit.hitCollider->gameObject->GetName() << ", Dist: " << hit.distance
						<< ", Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ") \n";

					SetSelected(hit.hitCollider->gameObject.Get());
				}
			}

			// Windows
			m_Ew->SceneHierarchy();
			m_Ew->Inspector();
			m_Ew->MenuBar();

			ImGui::ShowDemoWindow();

			ProcessDestroyQueue();
		}

		void LateUpdate() override
		{
			// Move camera
			m_SceneCameraGameObject->LateUpdate();
		}

		void SetSelected(GameObject* selected)
		{
			m_Selected = selected;
		}

		GameObject* GetSelected() const
		{
			return m_Selected;
		}
	};

}
