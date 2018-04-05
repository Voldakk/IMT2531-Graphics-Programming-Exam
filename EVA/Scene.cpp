#include "Scene.hpp"

#include "GL/glew.h"

#include "Application.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace EVA
{
	Material shadowMaterial;
	Material shadowMaterialInstanced;

	Scene::Scene()
	{
		shadowMaterial.shader = std::make_shared<Shader>("shadow.vert", "shadow.frag");
		shadowMaterialInstanced.shader = std::make_shared<Shader>("shadow_instanced.vert", "shadow_instanced.frag");
	}

	void Scene::Update(const float deltaTime)
	{
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Update(deltaTime);
		}
	}

	void Scene::Render()
	{
		for (auto& light : m_Lights)
		{
			if (light->GetType() == LightType::Directional)
			{
				// Shadow map
				glViewport(0, 0, light->GetShadwoSize(), light->GetShadwoSize());
				glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthMapFb());
				glClear(GL_DEPTH_BUFFER_BIT);
				RenderShadowMap(light->GetLightSpaceMatrix());
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else
			{
				
			}
		}

		// Scene
		const auto ws = Application::GetWindowSize();
		glViewport(0, 0, ws.x, ws.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, m_Lights[0]->GetDepthMap());
		RenderScene(m_Lights[0]->GetLightSpaceMatrix());
	}

	std::shared_ptr<GameObject> Scene::CreateGameObject()
	{
		auto gameObject = std::make_shared<GameObject>(this);

		m_GameObjects.push_back(gameObject);

		return gameObject;
	}

	std::shared_ptr<Light> Scene::CreateLight(LightType type, const bool shadows, const unsigned int shadowSize)
	{
		auto light = std::make_shared<Light>(type, shadows, shadowSize);

		m_Lights.push_back(light);

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

	void Scene::RenderScene(const glm::mat4 lightSpaceMatrix)
	{
		if (skybox != nullptr)
		{
			skybox->Render();
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
					if (!meshes[0]->GetMesh()->isStatic ||
						(meshes[0]->GetMesh()->isStatic && !meshes[0]->GetMesh()->HasIbo()))
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
			}
			else // If not
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

	void Scene::RenderShadowMap(const glm::mat4 lightSpaceMatrix)
	{
		for (auto &materials : m_MeshRenderers)
		{
			// If he material should use GPU instancing
			if (materials[0][0]->GetMaterial()->enableInstancing)
			{
				// Set material / shader
				shadowMaterialInstanced.Activate(this, nullptr);
				shadowMaterialInstanced.shader->SetUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);

				// For each mesh
				for (auto &meshes : materials)
				{
					// If the mesh isn't static or the static mesh is missing the ibo
					if (!meshes[0]->GetMesh()->isStatic ||
						(meshes[0]->GetMesh()->isStatic && !meshes[0]->GetMesh()->HasIbo()))
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
			}
			else // If not
			{
				// For each mesh
				for (auto &meshes : materials)
				{
					// For each MeshRenderer that use the mesh
					for (auto &meshRenderer : meshes)
					{
						// Render the mesh at the MeshRenderers position
						shadowMaterial.Activate(this, meshRenderer->GetGameObject()->GetTransform().get());
						shadowMaterial.shader->SetUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);
						meshRenderer->GetMesh()->Draw();
					}
				}
			}
		}
	}
}
