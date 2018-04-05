#include "Game.hpp"

#include "TileMap.hpp"
#include "Ghost.hpp"
#include "Pacman.hpp"

Game::Game()
{
    // Skybox
    skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

    // A directional light that fits the skybox
    auto directionalLight = CreateLight(EVA::LightType::Directional, true);
    directionalLight->SetRotation({ 30.0f, -80.0});

    //auto pointLight = CreateLight(EVA::LightType::Point, true, 128);
    //pointLight->position = { 11.0f, 1.0f, 10.0f };
    //pointLight->color = { 10.0f, 0.0f, 0.0f };
    //pointLight->attenuation = 0.5f;

    // Tilemap
    auto tileMapGo = CreateGameObject();
    auto tileMap = tileMapGo->AddComponent<TileMap>();
    tileMap->ReadFile("./assets/levels/level1.txt");

    // Camera
	auto goCamera = CreateGameObject();
	EVA::Application::mainCamera = goCamera->AddComponent<EVA::Camera>();
    goCamera->GetTransform()->SetPosition({tileMap->Width(), 5.0f, 20.0f});
    goCamera->GetTransform()->SetRotation({0.0f, glm::radians(-90.0f), 0.0f});

	// Pacman
	auto pacman = CreateGameObject();
	pacman->AddComponent<Pacman>();
	pacman->GetTransform()->SetPosition({ 0.0f, 1.0f, 0.0f });

	// Ghost
	auto ghost = CreateGameObject();
	ghost->AddComponent<Ghost>();
	ghost->GetTransform()->SetPosition({ 2.0f, 1.0f, 0.0f });
}


void Game::Update(const float deltaTime)
{
    Scene::Update(deltaTime);
}