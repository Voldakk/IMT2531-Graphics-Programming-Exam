#include "Game.hpp"

#include "Cube.hpp"

Game::Game()
{
	cube = new Cube();
	cube->AddTexture(TextureType::Diffuse, "../assets/uv.png");
	cube->position = { 0.0f, 0.0f, 2.0f };
	cube->Create();

	cube2 = new Cube();
	cube2->AddTexture(TextureType::Diffuse, "../assets/uv.png");
	cube2->position = { 2.0f, 0.0f, 3.0f };
	cube2->Create();

}


Game::~Game()
{
	delete(cube);
	delete(cube2);
}

void Game::Update(float deltaTime)
{
	cube->rotation.y += deltaTime /2;
}

void Game::Render()
{
	cube->Render();
	cube2->Render();
}
