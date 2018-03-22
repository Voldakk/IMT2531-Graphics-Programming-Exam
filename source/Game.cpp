#include "Game.hpp"

#include <memory>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Primitive.hpp"
#include "Input.hpp"
#include "Application.hpp"
#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Material.hpp"

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
	const auto unlitTextureShader = std::make_shared<UnlitTextureShader>();

	// Materials
	const auto materialUv = std::make_shared<Material>();
	materialUv->AddTexture(TextureType::Diffuse, "./assets/uv.png");
	materialUv->shader = standardShader;

	const auto materialYellow = std::make_shared<Material>();
	materialYellow->AddTexture(TextureType::Diffuse, "./assets/uv2.png");
	materialYellow->shader = standardShader;

	const auto materialGrid = std::make_shared<Material>();
	materialGrid->AddTexture(TextureType::Diffuse, "./assets/grid.png");
	materialGrid->shader = unlitTextureShader;

	// Meshes
	const auto meahCube = std::make_shared<Primitive>(PrimitiveType::Cube);
	const auto meshSphere = std::make_shared<Primitive>(PrimitiveType::Sphere);
	const auto meshCone = std::make_shared<Primitive>(PrimitiveType::Cone);
	const auto meshPlane = std::make_shared<Primitive>(PrimitiveType::Plane);

	// Cube
	goCube = CreateGameObject();
	goCube->transform->SetPosition({ 0.0f, 1.0f, 0.0f });

	auto mr = goCube->AddComponent<MeshRenderer>();
	mr->material = materialUv;
	mr->mesh = meahCube;

	// Cone
	goCone = CreateGameObject();
	goCone->transform->SetPosition({ 0.0f, 0.0f, 5.0f });
	goCone->SetParent(goCube);

	mr = goCone->AddComponent<MeshRenderer>();
	mr->material = materialUv;
	mr->mesh = meshCone;

	// Cone
	auto goCone2 = CreateGameObject();
	goCone2->transform->SetPosition({ 0.0f, 0.0f, 2.0f });
	goCone2->transform->SetScale(glm::vec3(0.2f));
	goCone2->SetParent(goCone);

	mr = goCone2->AddComponent<MeshRenderer>();
	mr->material = materialUv;
	mr->mesh = meshCone;

	// Ground
	auto goGround = CreateGameObject();
	goGround->transform->SetScale({ 10.0f, 1.0f, 10.0f });

	mr = goGround->AddComponent<MeshRenderer>();
	mr->material = materialGrid;
	mr->mesh = meshPlane;

	// Camera
	auto goCamera = CreateGameObject();
	Application::mainCamera = goCamera->AddComponent<Camera>();
}

void Game::Update(const float deltaTime) 
{
	Scene::Update(deltaTime);

	const auto rotation = 1.0f * deltaTime;
	goCube->transform->Rotate({ 0.0f, rotation, 0.0f });
	goCone->transform->Rotate({ 0.0f, -2 * rotation, 0.0f });
}