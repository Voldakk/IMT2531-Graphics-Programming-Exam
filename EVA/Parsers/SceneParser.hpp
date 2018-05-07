#pragma once

#include "GameObjectParser.hpp"

namespace EVA
{
	
	/**
	 * \brief Parser for scene asset files
	 */
	class SceneParser
	{

	public:

		/**
		 * \brief Loads to contetns of a scene asset file to a scene
		 * \param scene The scene to load to 
		 * \param path The path to the file 
		 */
		static void Load(Scene* scene, const std::string& path);

		/**
		 * \brief Saves a scene to a scene asset file
		 * \param scene The scene to save
		 * \param path The path to the file 
		 */
		static void Save(Scene* scene, const std::string& path);
	};

	inline void SceneParser::Load(Scene* scene, const std::string& path)
	{
		const auto sd = Json::Open(path);
		auto& d = *sd;

		// Skybox
		if (d.HasMember("skybox") && d["skybox"].IsObject())
		{
			scene->skybox = std::make_unique<Skybox>(DataObject(d["skybox"]));
		}

		// Lights
		if (d.HasMember("lights") && d["lights"].IsArray())
		{
			auto lights = d["lights"].GetArray();

			for (unsigned int i = 0; i < lights.Size(); ++i)
			{
				scene->CreateLight(DataObject(lights[i]));
			}
		}

		// Game objects
		if (d.HasMember("gameObjects") && d["gameObjects"].IsArray())
		{
			auto gameObjects = d["gameObjects"].GetArray();

			for (unsigned int i = 0; i < gameObjects.Size(); ++i)
			{
				auto& go = gameObjects[i];

				GameObject* gameObject = nullptr;

				// Prefab
				if (go.HasMember("prefab") && go["prefab"].IsString())
					gameObject = GameObjectParser::Load(scene, go["prefab"].GetString());
				else
					gameObject = scene->CreateGameObject().get();

				// Load the rest of the components
				GameObjectParser::Load(gameObject, go);
			}
		}
	}

	inline void SceneParser::Save(Scene* scene, const std::string& path)
	{
		// Create document
		Json::Document d;
		d.SetObject();

		auto& a = d.GetAllocator();

		// Skybox
		if (scene->skybox != nullptr)
		{
			// Create a data object
			Json::Value skyboxValue;
			skyboxValue.SetObject();
			DataObject data(skyboxValue, &a);

			// Save the skybox data
			scene->skybox->Save(data);

			// Add it to the document
			d.AddMember("skybox", skyboxValue, a);
		}

		// Lights
		const auto lights = scene->GetLights();
		if (!lights.empty())
		{
			// Create array
			Json::Value lightsArray;
			lightsArray.SetArray();

			// For each light
			for (auto& light : lights)
			{
				// Create a data object
				Json::Value lightValue;
				lightValue.SetObject();
				DataObject data(lightValue, &a);

				// Save light data
				light->Save(data);

				// Add to array
				lightsArray.PushBack(lightValue, a);
			}

			// Add the lights array to the document
			d.AddMember("lights", lightsArray, a);
		}

		// Game objects
		const auto& gameObjects = scene->GetGameObjects();
		if(gameObjects.size() > 1)
		{
			// Create array
			Json::Value gameObjectArray;
			gameObjectArray.SetArray();

			for (auto& gameObject : gameObjects)
			{
				if(gameObject->transform->parent.Get() == nullptr && gameObject->GetName() != "EVA::EditorCamera")
				{
					// Create a data object
					Json::Value gameObjectValue;
					gameObjectValue.SetObject();

					// Save game object data
					GameObjectParser::Save(gameObject.get(), gameObjectValue, a);

					// Add to array
					gameObjectArray.PushBack(gameObjectValue, a);
				}
			}

			// Add the game objects array to the document
			d.AddMember("gameObjects", gameObjectArray, a);
		}

		// Save file
		Json::Save(&d, path);
	}
}
