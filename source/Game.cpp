#include "Game.hpp"

#include <memory>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Mesh.hpp"
#include "Input.hpp"
#include "Application.hpp"
#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include <iomanip>

Game::Game()
{
	// Skybox
	//skybox = std::make_unique<Skybox>("./assets/testsky/", ".png");
	skybox = std::make_unique<Skybox>("./assets/ame_ash/", ".tga");

	// Make the light fit the skybox
	light.position = { 0.0f, 100.0f, -1000.0f };

	// Camera
	auto goCamera = CreateGameObject();
	goCamera->transform->SetPosition({ 0.0f, 1.0f, 0.0f });
	Application::mainCamera = goCamera->AddComponent<Camera>();

	// Shaders
	const auto standardShader = std::make_shared<StandardShader>();
	const auto standardInstancedShader = std::make_shared<StandardInstancedShader>();

	// Materials
	const auto materialUv = std::make_shared<Material>();
	materialUv->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	materialUv->shader = standardShader;

	const auto materialUvInstanced = std::make_shared<Material>();
	materialUvInstanced->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	materialUvInstanced->shader = standardInstancedShader;
	materialUvInstanced->enableInstancing = true;

	// Tiles
	const auto tileMeshes = Mesh::LoadMultiple("./assets/tiles/tile.obj");
	auto x = 0;
	for (const auto& tileMesh : tileMeshes)
	{
		auto goTile = CreateGameObject();
		goTile->transform->SetPosition({ x * 3, 1.0f, 0.0f });

		auto mr = goTile->AddComponent<MeshRenderer>();
		mr->Set(tileMesh, materialUv);

		x++;
	}
	
}

int frameCounter = 0;
float timeCounter = 0;

void Game::Update(const float deltaTime) 
{
	Scene::Update(deltaTime);

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