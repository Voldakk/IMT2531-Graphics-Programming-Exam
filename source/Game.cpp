#include "Game.hpp"

#include <memory>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Primitive.hpp"
#include "Input.hpp"
#include "Application.hpp"
#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include <iomanip>

std::shared_ptr<GameObject> goCube;
std::shared_ptr<GameObject> goCone;

Game::Game()
{
	// Skybox
	skybox = std::make_unique<Skybox>("./assets/ame_ash/", ".tga");

	// Make the light fit the skybox
	light.position = { 0.0f, 100.0f, -1000.0f };

	// Shaders
	const auto standardShader = std::make_shared<StandardShader>();
	const auto standardInstancedShader = std::make_shared<StandardInstancedShader>();

	const auto unlitTextureShader = std::make_shared<UnlitTextureShader>();
	const auto unlitTextureInstancedShader = std::make_shared<UnlitTextureInstancedShader>();

	// Materials
	const auto materialUv = std::make_shared<Material>();
	materialUv->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	materialUv->shader = standardShader;

	const auto materialUvInstanced = std::make_shared<Material>();
	materialUvInstanced->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	materialUvInstanced->shader = standardInstancedShader;
	materialUvInstanced->enableInstancing = true;

	const auto materialUvInstancedUnlit = std::make_shared<Material>();
	materialUvInstancedUnlit->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	materialUvInstancedUnlit->shader = unlitTextureInstancedShader;
	materialUvInstancedUnlit->enableInstancing = true;

	const auto materialYellow = std::make_shared<Material>();
	materialYellow->AddTexture(TextureType::Diffuse, "./assets/uv2.png");
	materialYellow->shader = unlitTextureShader;

	const auto materialGrid = std::make_shared<Material>();
	materialGrid->AddTexture(TextureType::Diffuse, "./assets/grid.png");
	materialGrid->shader = unlitTextureShader;

	// Meshes
	const auto meshCube = std::make_shared<Primitive>(PrimitiveType::Cube);
	const auto meshCone = std::make_shared<Primitive>(PrimitiveType::Cone);
	const auto meshPlane = std::make_shared<Primitive>(PrimitiveType::Plane);

	const auto meshSphere_STATIC = std::make_shared<Primitive>(PrimitiveType::Sphere);
	meshSphere_STATIC->isStatic = true;
	
	// Cube
	goCube = CreateGameObject();
	goCube->transform->SetPosition({ 0.0f, 1.0f, 0.0f });

	auto mr = goCube->AddComponent<MeshRenderer>();
	mr->Set(meshCube, materialUv);

	// Cone
	goCone = CreateGameObject();
	goCone->transform->SetPosition({ 0.0f, 0.0f, 5.0f });
	goCone->SetParent(goCube);

	mr = goCone->AddComponent<MeshRenderer>();
	mr->Set(meshCone, materialUv);

	// Cone
	auto goCone2 = CreateGameObject();
	goCone2->transform->SetPosition({ 0.0f, 0.0f, 2.0f });
	goCone2->transform->SetScale(glm::vec3(0.2f));
	goCone2->SetParent(goCone);

	mr = goCone2->AddComponent<MeshRenderer>();
	mr->Set(meshCone, materialUv);

	// Ground
	auto goGround = CreateGameObject();
	goGround->transform->SetScale({ 10.0f, 1.0f, 10.0f });

	mr = goGround->AddComponent<MeshRenderer>();
	mr->Set(meshPlane, materialGrid);
	
	// Camera
	auto goCamera = CreateGameObject();
	Application::mainCamera = goCamera->AddComponent<Camera>();

	
	// Spheres
	for (int x = 0; x < 100; x++)
	{
		for (int z = 0; z < 100; z++)
		{
			for (int y = 0; y < 5; y++)
			{
				auto goSphere = CreateGameObject();
				goSphere->transform->SetPosition({ x + 11, y, z });
				goSphere->transform->SetScale(glm::vec3(0.5f));

				auto mr = goSphere->AddComponent<MeshRenderer>();
				mr->Set(meshSphere_STATIC, materialUvInstanced);
			}
		}
	}
}

int frameCounter = 0;
float timeCounter = 0;

void Game::Update(const float deltaTime) 
{
	Scene::Update(deltaTime);

	const auto rotation = 1.0f * deltaTime;
	goCube->transform->Rotate({ 0.0f, rotation, 0.0f });
	goCone->transform->Rotate({ 0.0f, -2 * rotation, 0.0f });


	frameCounter++;
	timeCounter += deltaTime;
	if (frameCounter == 10)
	{
		std::cout << std::fixed;
		std::cout.precision(1);

		std::cout << "FPS:" << std::setw(5) << 10.0f / timeCounter << "\n";

		frameCounter = 0;
		timeCounter = 0;
	}
}