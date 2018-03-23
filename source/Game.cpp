#include "Game.hpp"

#include <memory>
#include <iostream>
#include <iomanip>

#include "EVA-Engine/Application.hpp"

#include "TileMap.hpp"

Game::Game()
{
	// Skybox
	//skybox = std::make_unique<Skybox>("./assets/testsky/", ".png");
	skybox = std::make_unique<Skybox>("./assets/ame_ash/", ".tga");

	// Make the light fit the skybox
	light.position = { 0.0f, 100.0f, -1000.0f };

	// Camera
	auto goCamera = CreateGameObject();
	Application::mainCamera = goCamera->AddComponent<Camera>();

	// Tilemap
	auto tileMapGo = CreateGameObject();
	auto tileMap = tileMapGo->AddComponent<TileMap>();
	tileMap->ReadFile("./assets/levels/level1.txt");

	// Position the camera 
	goCamera->transform->SetPosition({ tileMap->Width(), 20.0f, 0.0f });
	goCamera->transform->SetRotation({ 0.0f, glm::radians(90.0f), 0.0f });
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