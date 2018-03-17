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


Game::~Game()
{
}

void Game::Update(float deltaTime)
{
	//cube->rotation.y += deltaTime /2;

	Application::camera.Update(deltaTime);

	/* Input testing

	if (Input::MouseButtonDown(GLFW_MOUSE_BUTTON_1))
		std::cout << "Mouse 1 Down \n";
	if (Input::MouseButton(GLFW_MOUSE_BUTTON_1))
		std::cout << "Mouse 1 \n";
	if (Input::MouseButtonUp(GLFW_MOUSE_BUTTON_1))
		std::cout << "Mouse 1 Up \n";

	if (Input::MouseButtonDown(GLFW_MOUSE_BUTTON_2))
		std::cout << "Mouse 2 Down \n";
	if (Input::MouseButton(GLFW_MOUSE_BUTTON_2))
		std::cout << "Mouse 2 \n";
	if (Input::MouseButtonUp(GLFW_MOUSE_BUTTON_2))
		std::cout << "Mouse 2 Up \n";

	glm::vec2 scroll = Input::GetScroll();
	if (scroll.x != 0.0f)
		std::cout << "Scroll X: " << scroll.x << " \n";
	if (scroll.y != 0.0f)
		std::cout << "Scroll Y: " << scroll.y << " \n";

	glm::vec2 mousePos = Input::MouseMovement();
	if(mousePos.x != 0.0f || mousePos.y != 0.0f)
		std::cout << "Mouse pos - x: " << std::to_string(mousePos.x) << ", y: " << std::to_string(mousePos.y) << " \n";

	*/
}

void Game::Render()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Render();
	}
}
