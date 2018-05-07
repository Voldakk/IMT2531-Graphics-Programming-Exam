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
		static void Load(Scene* scene, const std::string& path)
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

					GameObject * gameObject = nullptr;

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

		static void Save(Scene* scene, const std::string& path)
		{
			// Create document
			Json::Document d;
			d.SetObject();

			auto& a = d.GetAllocator();

			// Skybox
			if(scene->skybox != nullptr)
			{
				Json::Value skyboxValue;
				skyboxValue.SetObject();
				DataObject data(skyboxValue, &a);

				scene->skybox->Save(data);

				d.AddMember("skybox", skyboxValue, a);
			}

			// Lights
			const auto lights = scene->GetLights();
			if (!lights.empty())
			{

				Json::Value lightsArray;
				lightsArray.SetArray();
				for (auto& light : lights)
				{
					Json::Value lightValue;
					lightValue.SetObject();
					DataObject data(lightValue, &a);

					light->Save(data);

					lightsArray.PushBack(lightValue, a);
				}

				d.AddMember("lights", lightsArray, a);
			}

			// Game objects


			// Save file
			Json::Save(&d, path);
		}
	};

}
