#pragma once

#include "../Scene.hpp"

#include <iostream>
#include <memory>

#include "EditorWindows.hpp"
#include "../Parsers/SceneParser.hpp"
#include "../Components/SceneCamera.hpp"
#include "../Physics.hpp"

namespace EVA
{

	class SceneEditor : public Scene
	{
		GameObject* m_SceneCameraGameObject;
		SceneCamera* m_SceneCamera;

		std::unique_ptr<EditorWindows> m_Ew;

		const char* m_TemplateScenePath = "./assets/standard assets/scenes/template.scene";

	public:

		void Clear()
		{
			m_GameObjects.clear();
			m_NameMap.clear();
			m_NameCounter = 0;
			m_Lights.clear();
			skybox = nullptr;
			m_Ew->SelectGameObject(nullptr);
			m_Started = false;

			// Camera
			m_SceneCameraGameObject = CreateGameObject().get();
			m_SceneCameraGameObject->SetName("EVA::EditorCamera");
			m_SceneCameraGameObject->AddComponent<Camera>();
			m_SceneCamera = m_SceneCameraGameObject->AddComponent<SceneCamera>();

			m_SceneCameraGameObject->transform->SetPosition({-5.0f, 5.0f, -5.0f});
			m_SceneCamera->yaw = -45.0f;
			m_SceneCamera->pitch = -30.0f;
			m_SceneCamera->FreeLook::Update(0.0f);
		}

		void LoadTemplate()
		{
			SceneParser::Load(this, m_TemplateScenePath);
		}

		explicit SceneEditor(const FS::path& path = "")
		{
			// Windows
			m_Ew = std::make_unique<EditorWindows>(this);

			Clear();

			if (path.empty())
				LoadTemplate();
			else
				SceneParser::Load(this, path);
		}

		void Update(const float deltaTime) override
		{
			// Move camera
			m_SceneCameraGameObject->Update(deltaTime);

			// Check for clicked object
			if (Input::MouseButtonDown(Input::MouseLeft) && !ImGui::IsMouseHoveringAnyWindow())
			{
				const auto mousePos = Input::MousePosition();
				const auto ray = Physics::ScreenPosToWorldRay(mousePos, Application::mainCamera);

				RaycastHit hit;
				if (Physics::Raycast(ray, hit, this))
				{
					std::cout << "Name: " << hit.hitCollider->gameObject->GetName() << ", Dist: " << hit.distance
						<< ", Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ") \n";

					m_Ew->SelectGameObject(hit.hitCollider->gameObject.Get());
				}
			}

			// Windows
			m_Ew->SceneHierarchy();
			m_Ew->Inspector();
			m_Ew->MenuBar();
			m_Ew->AssetBrowser();

			//ImGui::ShowDemoWindow();

			// Usualy done in Scene::Update
			ProcessDestroyQueue();
		}

		void LateUpdate() override
		{
			// Move camera
			if(m_SceneCameraGameObject != nullptr)
				m_SceneCameraGameObject->LateUpdate();
		}
	};

}
