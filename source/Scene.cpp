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

	for (auto meshRenderers : meshRendererses)
	{
		for (auto meshRenderer : meshRenderers)
		{
			meshRenderer->Render();
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
	for (auto& meshRenderers : meshRendererses)
	{
		if(meshRenderers[0]->material == meshRenderer->material)
		{
			meshRenderers.push_back(meshRenderer);
			return;
		}
	}

	std::vector<MeshRenderer*> meshRenderers;
	meshRenderers.push_back(meshRenderer);
	meshRendererses.push_back(meshRenderers);
}