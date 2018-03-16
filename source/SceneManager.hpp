#pragma once
#include <vector>
#include <memory>

#include "Scene.hpp"

// Manages the active scenes in the game
class SceneManager
{
    public:
		/// <summary>Loads a scene ontop of any currently active scenes</summary>
		/// <param name="scene">The scene to be loaded</param>  
        static void LoadScene(std::shared_ptr<Scene> scene);

		/// <summary>Unloads scene</summary>
		/// <param name="scene">The scene to be unloaded</param>  
        static void UnloadScene(std::shared_ptr<Scene> scene);

		/// <summary>Unloads all active scenes and loads a new scene</summary>
		/// <param name="scene">The new scene</param>  
		static void ChangeScene(std::shared_ptr<Scene> scene);

		/// <summary>Updates all active scenes</summary>
		/// <param name="deltaTime">The time in seconds between frames</param>
        static void Update(float deltaTime);

		/// <summary>Renders all active scenes</summary>
        static void Render();

	private:
        static std::vector<std::shared_ptr<Scene>> scenes;

		/// <summary>Findes the index of the scene in the scenes vector</summary>
		/// <param name="path">The scene to find</param>  
		/// <returns>The index of the scene. Return -1 if the scene was not found</returns>  
		static int GetIndex(std::shared_ptr<Scene> scene);
};