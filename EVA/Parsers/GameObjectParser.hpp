#pragma once

#include "EVA.hpp"
#include "Json.hpp"

namespace EVA
{

	class GameObjectParser
	{

	public:

		static GameObject* Load(GameObject* gameObject, json_generic& d)
		{
			// Transform
			gameObject->transform->Load(DataObject(d));

			// Components
			if (d.HasMember("components") && d["components"].IsArray())
			{
				auto components = d["components"].GetArray();

				for (unsigned int i = 0; i < components.Size(); ++i)
				{
					auto& c = components[i];

					const std::string id = c["id"].GetString();

					const auto component = ComponentMap::CreateComponent(id);
					if (component != nullptr)
					{
						component->SetScene(gameObject->scene.Get());
						component->Load(DataObject(c));
						gameObject->AttachComponent(component);
					}
				}
			}

			// Children
			if (d.HasMember("gameObjects") && d["gameObjects"].IsArray())
			{
				auto gameObjects = d["gameObjects"].GetArray();

				for (unsigned int i = 0; i < gameObjects.Size(); ++i)
				{
					auto& go = gameObjects[i];

					GameObject * child = nullptr;

					// Prefab
					if (go.HasMember("prefab") && go["prefab"].IsString())
						child = GameObjectParser::Load(gameObject->scene.Get(), go["prefab"].GetString());
					else
						child = gameObject->scene->CreateGameObject().get();

					// Load the rest of the components
					GameObjectParser::Load(child, go);

					child->SetParent(gameObject);
				}
			}

			gameObject->Start();

			return gameObject;
		}

		static GameObject* Load(Scene* scene, json_generic& d)
		{
			auto gameObject = scene->CreateGameObject();

			Load(gameObject.get(), d);

			return gameObject.get();
		}

		static GameObject* Load (Scene* scene, const std::string& path)
		{
			const auto sd = Json::Open(path);

			return Load(scene, *sd);
		}
	};
}