#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Scene.hpp"
#include "Input.hpp"

namespace EVA
{

// Manages the active scenes in the game
	class SceneManager
	{

		static std::vector<std::shared_ptr<Scene>> m_Scenes;

	public:

		/// <summary>Creates and loads a new scene ontop of any currently active scenes</summary>
		/// <returns>Returns a pointer to the newly created scene</returns>
		template<class T>
		static std::shared_ptr<T> CreateScene();

		/// <summary>Loads an existing scene ontop of any currently active scenes</summary>
		/// <param name="scene">The scene to be loaded</param>
		static void LoadScene(const std::shared_ptr<Scene> &scene);

		/// <summary>Unloads a scene</summary>
		/// <param name="scene">The scene to be unloaded</param>
		static void UnloadScene(const std::shared_ptr<Scene> &scene);

		/// <summary>Unloads all active scenes and loads an existing scene</summary>
		/// <param name="scene">The new scene</param>
		static void ChangeScene(const std::shared_ptr<Scene> &scene);

		/// <summary>Unloads all active scenes and creates a new scene</summary>
		/// <returns>Returns a pointer to the newly created scene</returns>
		template<class T>
		static std::shared_ptr<Scene> ChangeScene();

		/// <summary>Updates all active scenes</summary>
		/// <param name="deltaTime">The time in seconds between frames</param>
		static void Update(float deltaTime);

		/// <summary>Renders all active scenes</summary>
		static void Render();

	private:

		/// <summary>Findes the index of the scene in the scenes vector</summary>
		/// <param name="path">The scene to find</param>
		/// <returns>The index of the scene. Return -1 if the scene was not found</returns>
		static int GetIndex(const std::shared_ptr<Scene> &scene);

		/// <summary>Clears the list of scenes and restes the scenes self pointer</summary>
		static void ClearScenes();
	};

	template<class T>
	std::shared_ptr<T> SceneManager::CreateScene()
	{
		std::cout << "SceneManager::CreateScene - Creating scene: " << typeid(T).name() << " \n";
		std::shared_ptr<T> scene = std::make_shared<T>();

		LoadScene(scene);

		return scene;
	}

	template<class T>
	std::shared_ptr<Scene> SceneManager::ChangeScene()
	{
		std::cout << "SceneManager::ChangeScene - Changing scene: " << typeid(T).name() << " \n";
		ClearScenes();

		Input::Clear();

		return CreateScene<T>();
	}

}