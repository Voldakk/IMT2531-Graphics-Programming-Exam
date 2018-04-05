#include "Game.hpp"

#include "TileMap.hpp"

Game::Game()
{
    // Skybox
    skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

    // A directional light that fits the skybox
    auto directionalLight = CreateLight(EVA::LightType::Directional, true);
    directionalLight->SetRotation({ 30.0f, -80.0});

    auto pointLight = CreateLight(EVA::LightType::Point, true, 128);
    pointLight->position = { 11.0f, 1.0f, 10.0f };
    pointLight->color = { 10.0f, 0.0f, 0.0f };
    pointLight->attenuation = 0.5f;

	/*auto directionalLight2 = CreateLight(EVA::LightType::Directional, true);
	directionalLight2->SetRotation({ 30.0f, 80.0 });
	directionalLight2->color = glm::vec3(0.2f);*/

    // Tilemap
    auto tileMapGo = CreateGameObject();
    auto tileMap = tileMapGo->AddComponent<TileMap>();
    tileMap->ReadFile("./assets/levels/level1.txt");

    // Camera
	auto goCamera = CreateGameObject();
	EVA::Application::mainCamera = goCamera->AddComponent<EVA::Camera>();
    goCamera->GetTransform()->SetPosition({tileMap->Width(), 5.0f, 20.0f});
    goCamera->GetTransform()->SetRotation({0.0f, glm::radians(-90.0f), 0.0f});


	// Sphere
	auto m = std::make_shared<EVA::Material>();
	m->shader = std::make_shared<EVA::StandardShader>();
	const auto sphereMesh = EVA::Mesh::Primitive(EVA::PrimitiveType::Sphere);

	auto goS = CreateGameObject();
	goS->GetTransform()->SetPosition({ 11.4f, 0.5f, 7.0f });
	goS->GetTransform()->SetScale(glm::vec3(0.5f));
	auto mr = goS->AddComponent<EVA::MeshRenderer>();
	mr->Set(sphereMesh, m);

	goS = CreateGameObject();
	goS->GetTransform()->SetPosition({ 11.0f, 0.5f, 8.2f });
	goS->GetTransform()->SetScale(glm::vec3(0.5f));
	mr = goS->AddComponent<EVA::MeshRenderer>();
	mr->Set(sphereMesh, m);
}


void Game::Update(const float deltaTime)
{
    Scene::Update(deltaTime);
}