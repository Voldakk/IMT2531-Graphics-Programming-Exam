#include "Scene.hpp"

namespace EVA
{

	void Scene::Update(const float deltaTime)
	{
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Update(deltaTime);
		}
	}

	void Scene::Render()
	{
		if (m_skybox != nullptr)
		{
			m_skybox->Render();
		}

		for (auto &materials : m_MeshRenderers)
		{
			// If he material should use GPU instancing
			if (materials[0][0]->GetMaterial()->enableInstancing)
			{
				// Set material / shader
				materials[0][0]->GetMaterial()->Activate(this, nullptr);

				// For each mesh
				for (auto &meshes : materials)
				{
					// If the mesh isn't static or the static mesh is missing the ibo
					if (!meshes[0]->GetMesh()->m_IsStatic ||
						(meshes[0]->GetMesh()->m_IsStatic && !meshes[0]->GetMesh()->HasIbo()))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->GetGameObject()->GetTransform()->GetModelMatrix());
						}
						// Set the mesh ibo
						meshes[0]->GetMesh()->SetIbo(models);
					}

					// Draw the mesh
					meshes[0]->GetMesh()->DrawInstanced();
				}
			} else // If not
			{
				// For each mesh
				for (auto &meshes : materials)
				{
					// For each MeshRenderer that use the mesh
					for (auto &meshRenderer : meshes)
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
		auto gameObject = std::make_shared<GameObject>(this);

		m_GameObjects.push_back(gameObject);

		return gameObject;
	}

	std::shared_ptr<Light> Scene::CreateLight()
	{
		auto light = std::make_shared<Light>();

		m_lights.push_back(light);

		return light;
	}

	void Scene::RegisterMeshRenderer(MeshRenderer *meshRenderer)
	{
		for (auto &materials : m_MeshRenderers)
		{
			if (materials[0][0]->GetMaterial() == meshRenderer->GetMaterial())
			{
				for (auto &meshes : materials)
				{
					if (meshes[0]->GetMesh() == meshRenderer->GetMesh())
					{
						meshes.push_back(meshRenderer);
						return;
					}
				}

				std::vector<MeshRenderer *> meshes;
				meshes.push_back(meshRenderer);

				materials.push_back(meshes);

				return;
			}
		}

		std::vector<MeshRenderer *> meshes;
		meshes.push_back(meshRenderer);

		std::vector<std::vector<MeshRenderer *>> materials;
		materials.push_back(meshes);

		m_MeshRenderers.push_back(materials);
	}

}