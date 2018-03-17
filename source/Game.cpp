#include "Game.hpp"

#include <iostream>
#include <string>

#include "GLFW/glfw3.h"

#include "Primitive.hpp"
#include "Input.hpp"
#include "Application.hpp"

Game::Game()
{
	for (size_t i = 0; i <= PrimitiveType::Torus; i++)
	{
		Mesh mesh = Primitive((PrimitiveType)i);
		mesh.AddTexture(TextureType::Diffuse, "../assets/uv.png");
		mesh.position = { i * 3.0f, 0.0f, 2.0f };
		meshes.push_back(mesh);
	}
}

void Game::Update(float deltaTime)
{
	Application::camera.Update(deltaTime);
}

void Game::Render()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Render(this);
	}
}
