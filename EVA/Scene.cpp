#include "Scene.hpp"

#include "GL/glew.h"

#include "Application.hpp"
#include "ShaderManager.hpp"
#include <iostream>

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

	Scene::~Scene()
	{
		std::cout << "Scene::~Scene() - Deconstructing \n";
	}

	void Scene::Update(const float deltaTime)
	{
		// Update
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Update(deltaTime);
		}

		// Destroy
		ProcessDestroyQueue();
		
		// UI
		for (const auto& uiElement : m_UiElements)
		{
			uiElement->Update();
		}
	}

	void Scene::LateUpdate()
	{
		// Update
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->LateUpdate();
		}

		// Destroy
		ProcessDestroyQueue();
	}

	void Scene::Render()
	{

		// Shadows 
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

		// Render components
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Render();
		}

		// UI
		RenderUi();
	}

	std::shared_ptr<GameObject> Scene::CreateGameObject()
	{
		auto gameObject = std::make_shared<GameObject>(this);

		m_GameObjects.push_back(gameObject);

		return gameObject;
	}

	void Scene::DestroyGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr)
			return;

		m_DestroyQueue.push_back(gameObject);
	}

	std::shared_ptr<Light> Scene::CreateLight(LightType type, const bool shadows, const unsigned int shadowSize)
	{
		auto light = std::make_shared<Light>(type, shadows, shadowSize);

		m_Lights.push_back(light);

		return light;
	}

	void Scene::RegisterMeshRenderer(MeshRenderer *meshRenderer)
	{
		if (meshRenderer == nullptr)
			return;

		for (auto &materials : m_Materials)
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

		m_Materials.push_back(materials);
	}

	void Scene::RemoveMeshRenderer(MeshRenderer* removeMeshRenderer)
	{
		if (removeMeshRenderer == nullptr)
			return;

		for (unsigned int materialIndex = 0; materialIndex < m_Materials.size(); materialIndex++)
		{
			auto& material = m_Materials[materialIndex];
			if (material[0][0]->material == removeMeshRenderer->material)
			{
				for (unsigned int meshIndex = 0; meshIndex < material.size(); meshIndex++)
				{
					auto& mesh = material[meshIndex];
					for (unsigned int meshRendererIndex = 0; meshRendererIndex < mesh.size(); meshRendererIndex++)
					{
						const auto& meshRenderer = mesh[meshRendererIndex];
						if (meshRenderer == removeMeshRenderer)
						{
							mesh.erase(mesh.begin() + meshRendererIndex);
							if (mesh.empty())
							{
								material.erase(material.begin() + meshIndex);
								if (material.empty())
								{
									m_Materials.erase(m_Materials.begin() + materialIndex);
								}
							}
							return;
						}
					}
				}
			}
		}
	}

	void Scene::OnScreenResize()
	{
		for (auto& uiElement : m_UiElements)
		{
			if (*(uiElement->parent) == nullptr)
				uiElement->OnScreenResize();
		}
	}

	void Scene::RenderScene()
	{
		const auto ws = Application::GetWindowSize();
		glViewport(0, 0, ws.x, ws.y);

		if (skybox != nullptr)
		{
			skybox->Render();
		}

		for (auto &material : m_Materials)
		{
			// If he material should use GPU instancing
			if (material[0][0]->material->enableInstancing)
			{
				// Set material / shader
				material[0][0]->material->Activate(this, nullptr);

				// For each mesh
				for (auto &meshes : material)
				{
					// If the mesh isn't static or the static mesh is missing the ibo
					if (!meshes[0]->mesh->isStatic || meshes[0]->mesh->isDirty ||
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

						meshes[0]->mesh->isDirty = false;
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced();
				}
			}
			else // If not
			{
				// For each mesh
				for (auto &meshes : material)
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
		for (auto &materials : m_Materials)
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
					if (!meshes[0]->mesh->isStatic || meshes[0]->mesh->isDirty ||
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

						meshes[0]->mesh->isDirty = false;
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
		for (auto &materials : m_Materials)
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
					if (!meshes[0]->mesh->isStatic || meshes[0]->mesh->isDirty ||
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

						meshes[0]->mesh->isDirty = false;
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

	void Scene::RenderUi()
	{
		glDepthMask(GL_FALSE);
		for (const auto& uiElement : m_UiElements)
		{
			uiElement->Render();
		}
		glDepthMask(GL_TRUE);
	}

	void Scene::ProcessDestroyQueue()
	{
		for (auto gameObject : m_DestroyQueue)
		{
			for (unsigned int i = 0; i < m_GameObjects.size(); ++i)
			{
				if (m_GameObjects[i].get() == gameObject)
				{
					m_GameObjects.erase(m_GameObjects.begin() + i);
					break;;
				}
			}
		}
		m_DestroyQueue.clear();
	}
}
