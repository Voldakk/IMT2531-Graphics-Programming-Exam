#include "Game.hpp"

#include "Cube.hpp"

Game::Game()
{
	cube = new Cube();
}


Game::~Game()
{
	delete(cube);
}

void Game::Update(float deltaTime)
{
	cube->rotation.y += deltaTime;
}

void Game::Render()
{
	cube->Render();
}
