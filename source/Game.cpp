#include "Game.hpp"

#include "TileMap.hpp"

Game::Game()
{
    // Skybox
    m_skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

    // A directional light that fits the skybox
    auto directionalLight = CreateLight();
    directionalLight->Type = EVA::LightType::Directional;
    directionalLight->SetRotation({ 30.0f, -90.0});

    auto pointLight = CreateLight();
    pointLight->Type = EVA::LightType::Point;
    pointLight->Position = { 11.0f, 1.0f, 10.0f };
    pointLight->Color = { 10.0f, 0.0f, 0.0f };
    pointLight->Attenuation = 0.5f;

    // Camera
    auto goCamera = CreateGameObject();
    EVA::Application::m_MainCamera = goCamera->AddComponent<EVA::Camera>();

    // Tilemap
    auto tileMapGo = CreateGameObject();
    auto tileMap = tileMapGo->AddComponent<TileMap>();
    tileMap->ReadFile("./assets/levels/level1.txt");

    // Position the camera
    goCamera->GetTransform()->SetPosition({tileMap->Width(), 5.0f, 20.0f});
    goCamera->GetTransform()->SetRotation({0.0f, glm::radians(-90.0f), 0.0f});
}


void Game::Update(const float deltaTime)
{
    Scene::Update(deltaTime);
}