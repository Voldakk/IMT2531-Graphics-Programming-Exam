#include "Scene.hpp"

#include "GL/glew.h"

#include "Application.hpp"
#include "ShaderManager.hpp"

namespace EVA
{
	ShadowMaterial shadowMaterial;
	ShadowMaterial shadowMaterialInstanced;

	ShadowMaterial shadowMaterialCube;
	ShadowMaterial shadowMaterialCubeInstanced;

	Scene::Scene()
	{
		shadowMaterial.shader = ShaderManager::CreateOrGetShader("scene_shadow", "shadow.vert", "shadow.frag");
		shadowMaterialInstanced.shader = ShaderManager::CreateOrGetShader("scene_shadow_instanced", "shadow_instanced.vert", "shadow.frag");
	
		shadowMaterialCube.shader = ShaderManager::CreateOrGetShader("scene_shadow_cube", "shadow_cube.vert", "shadow_cube.frag", "shadow_cube.geom");
		shadowMaterialCubeInstanced.shader = ShaderManager::CreateOrGetShader("scene_shadow_cube_instanced", "shadow_cube_instanced.vert", "shadow_cube.frag", "shadow_cube.geom");
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
			if (light->GetType() == LightType::Directional && light->Shadows())
			{
				glViewport(0, 0, light->GetShadwoSize(), light->GetShadwoSize());
				glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthMapFb());
				glClear(GL_DEPTH_BUFFER_BIT);

				RenderShadowMap(light->GetLightSpaceMatrix());

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else if (light->GetType() == LightType::Point && light->Shadows())
			{
				glViewport(0, 0, light->GetShadwoSize(), light->GetShadwoSize());
				glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthMapFb());
				glClear(GL_DEPTH_BUFFER_BIT);
				
				RenderShadowCubeMap(light->GetShadowTransforms(), light->position, light->pointFarPlane);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}

		// Scene
		RenderScene();
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
			if (materials[0][0]->material == meshRenderer->material)
			{
				for (auto &meshes : materials)
				{
					if (meshes[0]->mesh == meshRenderer->mesh)
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

	void Scene::RenderScene()
	{
		const auto ws = Application::GetWindowSize();
		glViewport(0, 0, ws.x, ws.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (skybox != nullptr)
		{
			skybox->Render();
		}

		for (auto &materials : m_MeshRenderers)
		{
			// If he material should use GPU instancing
			if (materials[0][0]->material->enableInstancing)
			{
				// Set material / shader
				materials[0][0]->material->Activate(this, nullptr);

				// For each mesh
				for (auto &meshes : materials)
				{
					// If the mesh isn't static or the static mesh is missing the ibo
					if (!meshes[0]->mesh->isStatic ||
						(meshes[0]->mesh->isStatic && !meshes[0]->mesh->HasMbo()))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->transform->modelMatrix);
						}
						// Set the mesh ibo
						meshes[0]->mesh->SetMbo(models);
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced();
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
			// If the material should use GPU instancing
			if (materials[0][0]->material->enableInstancing)
			{
				// Set material / shader
				shadowMaterialInstanced.Activate(this, nullptr);
				shadowMaterialInstanced.shader->SetUniformMatrix4Fv("lightSpaceMatrix", lightSpaceMatrix);

				// For each mesh
				for (auto &meshes : materials)
				{
					// If the mesh isn't static or the static mesh is missing the ibo
					if (!meshes[0]->mesh->isStatic ||
						(meshes[0]->mesh->isStatic && !meshes[0]->mesh->HasMbo()))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->transform->modelMatrix);
						}
						// Set the mesh ibo
						meshes[0]->mesh->SetMbo(models);
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced();
				}
			}
			else // If not
			{
				shadowMaterial.Activate(this, nullptr);
				shadowMaterial.shader->SetUniformMatrix4Fv("lightSpaceMatrix", lightSpaceMatrix);

				// For each mesh
				for (auto &meshes : materials)
				{
					// For each MeshRenderer that use the mesh
					for (auto &meshRenderer : meshes)
					{
						// Render the mesh at the MeshRenderers position
						shadowMaterial.SetObjectUniforms(meshRenderer->transform.Get());
						meshRenderer->mesh->Draw();
					}
				}
			}
		}
	}

	void Scene::RenderShadowCubeMap(const std::vector<glm::mat4>& shadowMatrices, const glm::vec3 lightPos, const float farPlane)
	{
		for (auto &materials : m_MeshRenderers)
		{
			// If the material should use GPU instancing
			if (materials[0][0]->material->enableInstancing)
			{
				// Set material / shader
				shadowMaterialCubeInstanced.Activate(this, nullptr);
				shadowMaterialCubeInstanced.shader->SetUniform3Fv("lightPos", lightPos);
				shadowMaterialCubeInstanced.shader->SetUniform1F("farPlane", farPlane);
				for (unsigned int i = 0; i < shadowMatrices.size(); ++i)
				{
					shadowMaterialCubeInstanced.shader->SetUniformMatrix4Fv("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
				}

				// For each mesh
				for (auto &meshes : materials)
				{
					// If the mesh isn't static or the static mesh is missing the ibo
					if (!meshes[0]->mesh->isStatic ||
						(meshes[0]->mesh->isStatic && !meshes[0]->mesh->HasMbo()))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->transform->modelMatrix);
						}
						// Set the mesh ibo
						meshes[0]->mesh->SetMbo(models);
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced();
				}
			}
			else // If not
			{
				shadowMaterialCube.Activate(this, nullptr);
				shadowMaterialCube.shader->SetUniform3Fv("lightPos", lightPos);
				shadowMaterialCube.shader->SetUniform1F("farPlane", farPlane);
				for (unsigned int i = 0; i < shadowMatrices.size(); ++i)
				{
					shadowMaterialCube.shader->SetUniformMatrix4Fv("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
				}

				// For each mesh
				for (auto &meshes : materials)
				{
					// For each MeshRenderer that use the mesh
					for (auto &meshRenderer : meshes)
					{
						// Render the mesh at the MeshRenderers position
						shadowMaterialCube.SetObjectUniforms(meshRenderer->transform.Get());
						meshRenderer->mesh->Draw();
					}
				}
			}
		}
	}
}
