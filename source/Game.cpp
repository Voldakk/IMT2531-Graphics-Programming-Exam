#include "Game.hpp"

#include "TileMap.hpp"

Game::Game()
{
	// Skybox
	//skybox = std::make_unique<Skybox>("./assets/testsky/", ".png");
	skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

	// Make the light fit the skybox
	light.Position = { 0.0f, 100.0f, -1000.0f };

	// Camera
	auto goCamera = CreateGameObject();
	EVA::Application::m_MainCamera = goCamera->AddComponent<EVA::Camera>();

	// Tilemap
	auto tileMapGo = CreateGameObject();
	auto tileMap = tileMapGo->AddComponent<TileMap>();
	tileMap->ReadFile("./assets/levels/level1.txt");

	// Position the camera 
	goCamera->GetTransform()->SetPosition({ tileMap->Width(), 20.0f, 0.0f });
	goCamera->GetTransform()->SetRotation({ 0.0f, glm::radians(90.0f), 0.0f });
}



void Game::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
}