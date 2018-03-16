#include "SceneManager.hpp"

#include <vector>
#include <iostream>
#include <memory>

#include "Input.hpp"

std::vector<std::shared_ptr<Scene>> SceneManager::scenes;

void SceneManager::LoadScene(std::shared_ptr<Scene> scene)
{
    if(GetIndex(scene) == -1)
    {
		Input::ClearKeys();

        //std::cout << "SceneManager::LoadScene - Loading scene: " << typeid(*scene).name() << " \n";
		scene->self = scene;
        scenes.push_back(scene);
    }
}

void SceneManager::UnloadScene(std::shared_ptr<Scene> scene)
{
    int i = GetIndex(scene);
    if(i != -1)
    {
        //std::cout << "SceneManager::UnloadScene - Unloading scene: " << typeid(*scene).name() << " \n";
        scenes.erase(scenes.begin() + i);
    }
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> scene)
{
	//std::cout << "SceneManager::ChangeScene - Clearing scenes \n";
	scenes.clear();
    Input::ClearKeys();

	LoadScene(scene);
}

void SceneManager::Update(float deltaTime)
{
    for (size_t i = 0; i < scenes.size(); i++)
    {
        scenes[i]->Update(deltaTime);
    }
}

void SceneManager::Render()
{
    for (size_t i = 0; i < scenes.size(); i++)
    {
        scenes[i]->Render();
    }
}

int SceneManager::GetIndex(std::shared_ptr<Scene> scene)
{
	for (size_t i = 0; i < scenes.size(); i++)
	{
		if (scene == scenes[i])
			return i;
	}
	return -1;
}
