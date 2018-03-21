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
	
	// Meshes
	const auto cubeMesh = std::make_shared<Primitive>(PrimitiveType::Cube);
	const auto sphereMesh = std::make_shared<Primitive>(PrimitiveType::Sphere);
	const auto monkeyMesh = std::make_shared<Primitive>(PrimitiveType::Monkey);
	const auto planeMesh = std::make_shared<Primitive>(PrimitiveType::Plane);

	// Cube
	cube = CreateGameObject();

	auto mr = cube->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = cubeMesh;

	// Monkey
	auto monkey = CreateGameObject();
	monkey->transform->SetPosition({ 5.0f, 0.0f, 0.0f });
	monkey->SetParent(cube);

	mr = monkey->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = monkeyMesh;

	// Ground
	auto ground = CreateGameObject();
	ground->transform->SetPosition({ 0.0f, -1.0f, 0.0f });
	ground->transform->SetScale({ 10.0f, 1.0f, 10.0f });

	mr = ground->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = planeMesh;

	/*
	// Spheres
	for (size_t x = 0; x < 100; x++)
	{
		for (size_t z = 0; z < 100; z++)
		{
			std::shared_ptr<GameObject> sphere = CreateGameObject();
			sphere->transform->SetPosition({ x * 3.0f, 0.0f, z * 3.0f });

			mr = sphere->AddComponent<MeshRenderer>();

			mr->mesh = sphereMesh;

			if (x % 2 == 0)
			{
				mr->shader = shader;
				mr->material = material;
			}
			else
			{
				mr->shader = unlit;
				mr->material = material2;
			}
		}
	}
	*/
}

void Game::Update(const float deltaTime)
{
	Application::camera.Update(deltaTime);


	const auto rotation = 1.0f * deltaTime;
	cube->transform->Rotate({ 0.0f, rotation, 0.0f });
}