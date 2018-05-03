#pragma once

#include "GameObjectParser.hpp"

namespace EVA
{
	
	class SceneParser
	{

	public:

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
	};

}
