#pragma once

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
						gameObject = PrefabParser::Load(scene, go["prefab"].GetString());
					else
						gameObject = scene->CreateGameObject().get();

					PrefabParser::Load(gameObject, go);
				}
			}

		}
	};

}