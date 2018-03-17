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

	// Shader
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

	// Material
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->AddTexture(TextureType::Diffuse, "../assets/uv.png");
	
	// Meshes
	std::shared_ptr<Mesh> cubeMesh = std::make_shared<Primitive>(PrimitiveType::Cube);
	std::shared_ptr<Mesh> sphereMesh = std::make_shared<Primitive>(PrimitiveType::Sphere);

	// Cube
	std::shared_ptr<GameObject> cube = CreateGameObject();
	cube->transform->position = { 0.0f, 0.0f, 3.0f };

	std::shared_ptr<MeshRenderer> mr = cube->AddComponent<MeshRenderer>();
	mr->shader = shader;
	mr->material = material;
	mr->mesh = cubeMesh;

	// Spheres
	for (size_t i = 0; i < 10; i++)
	{
		std::shared_ptr<GameObject> sphere = CreateGameObject();
		sphere->transform->position = { i * 3.0f, 0.0f, 0.0f };

		mr = sphere->AddComponent<MeshRenderer>();
		mr->shader = shader;
		mr->material = material;
		mr->mesh = sphereMesh;
	}
}

void Game::Update(float deltaTime)
{
	Application::camera.Update(deltaTime);
}