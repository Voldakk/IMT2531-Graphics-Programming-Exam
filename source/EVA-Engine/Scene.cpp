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
		if(materials[0][0]->material->enableInstancing)
		{
			// Set material / shader
			materials[0][0]->material->Activate(materials[0][0]->gameObject->scene, nullptr);

			// For each mesh
			for (auto& meshes : materials)
			{
				if (!meshes[0]->mesh->isStatic || (meshes[0]->mesh->isStatic && !meshes[0]->mesh->HasIbo()) )
				{
					// Get all the model matrices
					std::vector<glm::mat4> models;
					models.reserve(meshes.size());

					for (auto& meshRenderer : meshes)
					{
						models.push_back(meshRenderer->gameObject->transform->GetModelMatrix());
					}

					meshes[0]->mesh->SetIbo(models);

					meshes[0]->mesh->instanceCount = models.size();
				}

				meshes[0]->mesh->DrawInstanced(meshes[0]->mesh->instanceCount);
			}
		}
		else
		{
			for (auto& meshes : materials)
			{
				for (auto& meshRenderer : meshes)
				{
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
		}
	}

	std::vector<MeshRenderer*> meshes;
	meshes.push_back(meshRenderer);

	std::vector<std::vector<MeshRenderer*>> materials;
	materials.push_back(meshes);

	meshRenderers.push_back(materials);
}