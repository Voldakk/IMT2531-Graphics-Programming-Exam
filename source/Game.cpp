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

std::shared_ptr<GameObject> cube;
std::shared_ptr<GameObject> cone;

Game::Game()
{
	// Skybox
	skybox = std::make_unique<Skybox>("./assets/ame_ash/", ".tga");

	// Make the light fit the skybox
	light.position = { 0.0f, 100.0f, -1000.0f };

	// Shaders
	const auto shader = std::make_shared<StandardShader>();
	const auto unlit = std::make_shared<UnlitTextureShader>();

	// Materials
	const auto material = std::make_shared<Material>();
	material->AddTexture(TextureType::Diffuse, "./assets/uv.png");

	const auto material2 = std::make_shared<Material>();
	material2->AddTexture(TextureType::Diffuse, "./assets/uv2.png");

	const auto materialGrid = std::make_shared<Material>();
	materialGrid->AddTexture(TextureType::Diffuse, "./assets/grid.png");
	
	// Meshes
	const auto cubeMesh = std::make_shared<Primitive>(PrimitiveType::Cube);
	const auto sphereMesh = std::make_shared<Primitive>(PrimitiveType::Sphere);
	const auto coneMesh = std::make_shared<Primitive>(PrimitiveType::Cone);
	const auto planeMesh = std::make_shared<Primitive>(PrimitiveType::Plane);

	// Cube
	cube = CreateGameObject();
	cube->transform->SetPosition({ 0.0f, 1.0f, 0.0f });

	auto mr = cube->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = cubeMesh;

	// Cone
	cone = CreateGameObject();
	cone->transform->SetPosition({ 0.0f, 0.0f, 5.0f });
	cone->SetParent(cube);

	mr = cone->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = coneMesh;

	// Cone
	auto cone2 = CreateGameObject();
	cone2->transform->SetPosition({ 0.0f, 0.0f, 2.0f });
	cone2->transform->SetScale(glm::vec3(0.2f));
	cone2->SetParent(cone);

	mr = cone2->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = coneMesh;

	// Ground
	auto ground = CreateGameObject();
	ground->transform->SetScale({ 10.0f, 1.0f, 10.0f });

	mr = ground->AddComponent<MeshRenderer>();
	mr->shader = unlit;
	mr->material = materialGrid;
	mr->mesh = planeMesh;

	// Camera
	auto camera = CreateGameObject();
	Application::mainCamera = camera->AddComponent<Camera>();
}

void Game::Update(const float deltaTime) 
{
	Scene::Update(deltaTime);

	const auto rotation = 1.0f * deltaTime;
	cube->transform->Rotate({ 0.0f, rotation, 0.0f });
	cone->transform->Rotate({ 0.0f, -2 * rotation, 0.0f });
}