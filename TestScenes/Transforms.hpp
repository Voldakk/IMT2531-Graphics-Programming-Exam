#pragma once

#include "EVA.hpp"
#include "EVA/Graphics.hpp"
#include "EVA/ResourceManagers.hpp"
#include "EVA/Components.hpp"

#include "../EVA/Physics.hpp"


namespace EVA_TEST
{

	class Transforms : public EVA::Scene
	{
		EVA::GameObject* m_Plane;
		EVA::GameObject* m_Cube;

	public:

		Transforms()
		{
			// Skybox
			skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

			// A directional light that fits the skybox
			auto directionalLight = CreateLight(EVA::Light::Directional, true);
			directionalLight->SetRotation({ 60.0f, 80.0f });
			directionalLight->directionalShadowDistance = 25.0f;
			directionalLight->color = { 1.0f, 1.0f, 1.0f };

			// Camera
			auto camera = CreateGameObject();
			camera->SetName("Camera");
			const auto cameraComponent = camera->AddComponent<EVA::Camera>();
			//camera->AddComponent<EVA::FreeLook>();
			EVA::Application::SetMainCamera(cameraComponent);
			EVA::Input::SetCursorMode(EVA::Input::Normal);
			camera->transform->SetPosition({ 0.0f, 4.0f, -10.0f });

			// Mesh
			const auto cubeModel = EVA::ModelManager::Primitive(EVA::ModelManager::Cube);
			const auto planeModel = EVA::ModelManager::Primitive(EVA::ModelManager::Plane);

			// Plane Material
			auto material = std::make_shared<EVA::Material>();
			material->SetTexture(EVA::Texture::Diffuse, "./assets/textures/grid.png");
			material->shader = EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");

			// Ground plane
			m_Plane = CreateGameObject().get();
			m_Plane->SetName("Plane");
			auto mr = m_Plane->AddComponent<EVA::MeshRenderer>();
			mr->Set(planeModel->GetMesh(0), material);

			m_Plane->transform->SetScale({10.0f, 0.010f, 10.0f});
			m_Plane->transform->SetPosition(EVA::YAXIS * -2.0f);

			// Cube Material
			material = std::make_shared<EVA::Material>();
			material->shader = EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");

			// Center cube
			m_Cube = CreateGameObject().get();
			m_Cube->SetName("Cube");
			mr = m_Cube->AddComponent<EVA::MeshRenderer>();
			mr->Set(cubeModel->GetMesh(0), material);
		}

		void Update(const float deltaTime) override
		{
			Scene::Update(deltaTime);

			if(EVA::Input::MouseButton(EVA::Input::MouseLeft))
			{
				const auto mousePos = EVA::Input::MousePosition();
				const auto ray = EVA::Physics::ScreenPosToWorldRay(mousePos, EVA::Application::mainCamera);

				EVA::RaycastHit hit;
				if (EVA::Physics::Raycast(ray, this, hit))
				{
					std::cout << "Name: " << hit.hitObject->GetName() << ", Dist: " << hit.distance << ", Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ") \n";
				}
					
			}
		}
	};
}