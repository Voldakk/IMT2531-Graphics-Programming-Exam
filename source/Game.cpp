#include "Game.hpp"

#include <memory>
#include <iostream>
#include <string>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Primitive.hpp"
#include "Input.hpp"
#include "Application.hpp"
#include "MeshRenderer.hpp"
#include "Shader.hpp"
#include "Material.hpp"

Game::Game()
{
	// Light
	light.position = { 10.0f, 0.0f, 5.0f };

	// Shaders
	std::shared_ptr<Shader> shader = std::make_shared<StandardShader>();
	std::shared_ptr<Shader> unlit = std::make_shared<UnlitTextureShader>();

	// Materials
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->AddTexture(TextureType::Diffuse, "../assets/uv.png");

	std::shared_ptr<Material> material2 = std::make_shared<Material>();
	material2->AddTexture(TextureType::Diffuse, "../assets/uv2.png");
	
	// Meshes
	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Primitive>(PrimitiveType::Cube);
	std::shared_ptr<Mesh> sphereMesh = std::make_shared<Primitive>(PrimitiveType::Sphere);

	// Cube
	std::shared_ptr<GameObject> cube = CreateGameObject();
	cube->transform->SetPosition({ -3.0f, 0.0f, -3.0f });

	std::shared_ptr<MeshRenderer> mr = cube->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = cubeMesh;

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
}

void Game::Update(float deltaTime)
{
	Application::camera.Update(deltaTime);
	std::cout << "FPS: " << 1.0f / deltaTime << "\n";
}