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
	/*
	for (size_t i = 0; i <= PrimitiveType::Torus; i++)
	{
		Mesh mesh = Primitive((PrimitiveType)i);
		mesh.AddTexture(TextureType::Diffuse, "../assets/uv.png");
		mesh.position = { i * 3.0f, 0.0f, 2.0f };
		meshes.push_back(mesh);
	}*/

	std::shared_ptr<GameObject> cube = std::make_shared<GameObject>();

	cube->transform->position = { 0.0f, 0.0f, 3.0f };
	cube->scene = this;

	MeshRenderer * mr = (MeshRenderer *)cube->AddComponent(std::make_shared<MeshRenderer>());

	mr->shader = std::make_shared<Shader>();

	mr->material = std::make_shared<Material>();
	mr->material->AddTexture(TextureType::Diffuse, "../assets/uv.png");

	mr->mesh = std::make_shared<Primitive>(PrimitiveType::Cube);

	

	gameObjects.push_back(cube);
}

void Game::Update(float deltaTime)
{
	Application::camera.Update(deltaTime);
}

void Game::Render()
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Render();
	}
}
