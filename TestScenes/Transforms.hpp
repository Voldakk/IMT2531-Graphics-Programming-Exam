#pragma once

#include "EVA.hpp"
#include "EVA/Graphics.hpp"
#include "EVA/ResourceManagers.hpp"
#include "EVA/Components.hpp"

#include "../EVA/Physics.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

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
				const auto windowSize = EVA::Application::GetWindowSize();
				const auto mousePos = EVA::Input::MousePosition();

				const auto ray = EVA::Physics::ScreenPosToWorldRay(mousePos, EVA::Application::mainCamera);

				EVA::GameObject* nearest = nullptr;
				auto minDist = 999999.0f;
				glm::vec3 point;

				for (const auto& gameObject : m_GameObjects)
				{
					if(gameObject->GetName() == "Camera")
						continue;;

					float intersectionDistance;
					const auto aabbMin = -gameObject->transform->scale;
					const auto aabbMax = gameObject->transform->scale;

					const auto rotationMatrix = glm::toMat4(gameObject->transform->orientation);
					const auto translationMatrix = glm::translate(glm::mat4(), gameObject->transform->position);
					const auto modelMatrix = translationMatrix * rotationMatrix;

					if (EVA::Physics::TestRayObbIntersection(
						ray,
						aabbMin,
						aabbMax,
						modelMatrix,
						intersectionDistance)
						) 
					{
						if(intersectionDistance < minDist)
						{
							minDist = intersectionDistance;
							nearest = gameObject.get();
							point = ray.origin + ray.direction * intersectionDistance;
						}
					}
				}

				if(nearest != nullptr)
					std::cout << "Name: " << nearest->GetName() << ", Dist: " << minDist << ", Point: (" << point.x << ", " << point.y << ", " << point.z << ") \n";
			}
		}
	};


}
