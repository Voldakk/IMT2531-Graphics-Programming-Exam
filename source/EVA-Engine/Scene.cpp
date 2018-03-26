#include "Scene.hpp"

#include <memory>

void Scene::Update(const float deltaTime)
{
	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void Scene::Render()
{
	if (skybox != nullptr)
	{
		skybox->Render();
	}

	for (auto& materials : meshRenderers)
	{
		// If he material should use GPU instancing
		if(materials[0][0]->material->enableInstancing)
		{
			// Set material / shader
			materials[0][0]->material->Activate(this, nullptr);

			// For each mesh
			for (auto& meshes : materials)
			{
				// If the mesh isn't static or the static mesh is missing the ibo
				if (!meshes[0]->mesh->isStatic || (meshes[0]->mesh->isStatic && !meshes[0]->mesh->HasIbo()) )
				{
					// Get the model matrices from all the objects
					std::vector<glm::mat4> models;
					models.reserve(meshes.size());

					for (auto& meshRenderer : meshes)
					{
						models.push_back(meshRenderer->gameObject->transform->GetModelMatrix());
					}
					// Set the mesh ibo
					meshes[0]->mesh->SetIbo(models);
				}

				// Draw the mesh
				meshes[0]->mesh->DrawInstanced();
			}
		}
		else // If not
		{
			// For each mesh
			for (auto& meshes : materials)
			{
				// For each MeshRenderer that use the mesh
				for (auto& meshRenderer : meshes)
				{
					// Render the mesh at the MeshRenderers position
					meshRenderer->Render();
				}
			}
		}
	}
}

std::shared_ptr<GameObject> Scene::CreateGameObject()
{
	auto gameObject = std::make_shared<GameObject>();

	gameObject->scene = this;

	gameObjects.push_back(gameObject);

	return gameObject;
}

void Scene::RegisterMeshRenderer(MeshRenderer* meshRenderer)
{
    std::string meshName = meshRenderer->mesh->name;
    std::string shaderName = typeid(*meshRenderer->material->shader).name();

	if(meshName != "15")
	{
		int i;
		i++;
	}

	for (auto& materials : meshRenderers)
	{
		if(materials[0][0]->material == meshRenderer->material)
		{
			for (auto& meshes : materials)
			{
				if (meshes[0]->mesh == meshRenderer->mesh)
				{
					meshes.push_back(meshRenderer);
					return;
				}
			}

			std::vector<MeshRenderer*> meshes;
			meshes.push_back(meshRenderer);

			materials.push_back(meshes);

			return;
		}
	}

	std::vector<MeshRenderer*> meshes;
	meshes.push_back(meshRenderer);

	std::vector<std::vector<MeshRenderer*>> materials;
	materials.push_back(meshes);

	meshRenderers.push_back(materials);
}