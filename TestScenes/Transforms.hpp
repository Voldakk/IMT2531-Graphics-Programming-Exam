#pragma once

#include "EVA.hpp"
#include "EVA/Graphics.hpp"
#include "EVA/ResourceManagers.hpp"
#include "EVA/Components.hpp"

namespace EVA_TEST
{

	class Transforms : public EVA::Scene
	{
		EVA::GameObject* m_CenterCube;

		EVA::GameObject* m_ChildCube2;

	public:

		Transforms()
		{
			// Skybox
			skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

			// A directional light that fits the skybox
			auto directionalLight = CreateLight(EVA::LightType::Directional, true);
			directionalLight->SetRotation({ 60.0f, 80.0f });
			directionalLight->directionalShadowDistance = 25.0f;
			directionalLight->color = { 1.0f, 1.0f, 1.0f };

			// Camera
			auto camera = CreateGameObject();
			const auto cameraComponent = camera->AddComponent<EVA::Camera>();
			camera->AddComponent<EVA::FreeLook>();
			EVA::Application::SetMainCamera(cameraComponent);
			EVA::Input::SetCursorMode(EVA::Input::Disabled);
			camera->transform->SetPosition({ 0.0f, 2.0f, -5.0f });

			// Mesh
			const auto cubeModel = EVA::ModelManager::Primitive(EVA::Cube);
			const auto planeModel = EVA::ModelManager::Primitive(EVA::Plane);

			// Plane Material
			auto material = std::make_shared<EVA::Material>();
			material->SetTexture(EVA::Diffuse, "./assets/textures/grid.png");
			material->shader = EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");

			// Ground plane
			auto plane = CreateGameObject();
			auto mr = plane->AddComponent<EVA::MeshRenderer>();
			mr->Set(planeModel->GetMesh(0), material);

			plane->transform->SetScale(10.0f);
			plane->transform->SetPosition(EVA::YAXIS * -2.0f);

			// Cube Material
			material = std::make_shared<EVA::Material>();
			material->shader = EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");

			// Center cube
			m_CenterCube = CreateGameObject().get();
			mr = m_CenterCube->AddComponent<EVA::MeshRenderer>();
			mr->Set(cubeModel->GetMesh(0), material);

			// Child cube
			auto childCube = CreateGameObject();
			mr = childCube->AddComponent<EVA::MeshRenderer>();
			mr->Set(cubeModel->GetMesh(0), material);

			childCube->SetParent(m_CenterCube);
			childCube->transform->SetScale(0.5f);
			childCube->transform->SetPosition(EVA::ZAXIS * 4.0f);

			// Child cube 2
			m_ChildCube2 = CreateGameObject().get();
			mr = m_ChildCube2->AddComponent<EVA::MeshRenderer>();
			mr->Set(cubeModel->GetMesh(0), material);

			m_ChildCube2->SetParent(m_CenterCube);
			m_ChildCube2->transform->SetScale(0.5f);
			m_ChildCube2->transform->SetPosition(EVA::ZAXIS * 8.0f);

			// Child cube 2.1
			auto childCube21 = CreateGameObject();

			mr = childCube21->AddComponent<EVA::MeshRenderer>();
			mr->Set(cubeModel->GetMesh(0), material);

			childCube21->SetParent(m_ChildCube2);
			childCube21->transform->SetScale(0.5f); // Effectively 0.5 * 0.5 = 0.25
			childCube21->transform->SetPosition(EVA::ZAXIS * 2.0f);
		}

		void Update(const float deltaTime) override
		{
			Scene::Update(deltaTime);

			m_CenterCube->transform->Rotate(EVA::YAXIS, deltaTime * 50.f);

			m_ChildCube2->transform->Rotate(EVA::XAXIS, deltaTime * 200.f);
		}
	};


}