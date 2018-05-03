#pragma once

#include "EVA.hpp"
#include "Json.hpp"

namespace EVA
{

	/**
	 * \brief Parser for game objects, prefabs and components
	 */
	class GameObjectParser
	{

	public:

		/**
		 * \brief Loads the data form the document into the GameObject
		 * \param gameObject The GameObject
		 * \param document The data
		 * \return A pointer to the GameObject 
		 */
		static GameObject* Load(GameObject* gameObject, Json::Generic& document);

		/**
		 * \brief Loads the data from the document into a new GameObject
		 * \param scene The Scene the GameObejct should be created in
		 * \param document The data
		 * \return A pointer to the GameObject 
		 */
		static GameObject* Load(Scene* scene, Json::Generic& document);

		/**
		 * \brief Loads the data form a prefab asset file into a new GameObject
		 * \param scene The Scene the GameObejct should be created in
		 * \param path The path to the file
		 * \return A pointer to the GameObject 
		 */
		static GameObject* Load(Scene* scene, const std::string& path);
	};

	inline GameObject* GameObjectParser::Load(GameObject* gameObject, Json::Generic& document)
	{
		// Transform
		gameObject->transform->Load(DataObject(document));

		if (document.HasMember("name") && document["name"].IsString())
			gameObject->SetName(document["name"].GetString());


		// Components
		if (document.HasMember("components") && document["components"].IsArray())
		{
			auto components = document["components"].GetArray();

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
		if (document.HasMember("gameObjects") && document["gameObjects"].IsArray())
		{
			auto gameObjects = document["gameObjects"].GetArray();

			for (unsigned int i = 0; i < gameObjects.Size(); ++i)
			{
				auto& go = gameObjects[i];

				GameObject* child = nullptr;

				// Prefab
				if (go.HasMember("prefab") && go["prefab"].IsString())
					child = Load(gameObject->scene.Get(), go["prefab"].GetString());
				else
					child = gameObject->scene->CreateGameObject().get();

				// Load the rest of the components
				Load(child, go);

				child->SetParent(gameObject);
			}
		}

		if (gameObject->scene->started)
		{
			gameObject->Awake();
			gameObject->Start();
		}

		return gameObject;
	}

	inline GameObject* GameObjectParser::Load(Scene* scene, Json::Generic& document)
	{
		auto gameObject = scene->CreateGameObject();

		Load(gameObject.get(), document);

		return gameObject.get();
	}

	inline GameObject* GameObjectParser::Load(Scene* scene, const std::string& path)
	{
		const auto sd = Json::Open(path);

		return Load(scene, *sd);
	}
}
