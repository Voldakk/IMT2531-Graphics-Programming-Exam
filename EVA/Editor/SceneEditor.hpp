#pragma once

#include "../Scene.hpp"

#include <iostream>
#include <memory>

#include "../Physics.hpp"
#include "../Components/SceneCamera.hpp"
#include "EditorWindows.hpp"
#include "../Parsers/SceneParser.hpp"
#include "../ModelManager.hpp"
#include "../MaterialManager.hpp"
#include "../ShaderManager.hpp"

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

			/*const auto material = MaterialManager::LoadMaterial("./assets/standard assets/materials/uv.mat");
			const auto mesh = ModelManager::Primitive(ModelManager::Monkey)->GetMesh(0);

			for (unsigned int x = 0; x < 1000; ++x)
			{
				auto g = CreateGameObject();
				g->SetName("Monkey " + std::to_string(x));
				g->transform->SetPosition({ x * 2.0f, 2.0f, 2.0f });
				auto mr = g->AddComponent<MeshRenderer>();
				mr->Set(mesh, material);
			}*/
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

					m_Ew->SelectGameObject(hit.hitCollider->gameObject.Get());
				}
			}

			// Windows
			m_Ew->SceneHierarchy();
			m_Ew->Inspector();
			m_Ew->MenuBar();
			m_Ew->AssetBrowser();

			//ImGui::ShowDemoWindow();

			ProcessDestroyQueue();
		}

		void LateUpdate() override
		{
			// Move camera
			m_SceneCameraGameObject->LateUpdate();
		}
	};

}
