#include "Scene.hpp"

#include <iostream>

#include "GL/glew.h"

#include "Application.hpp"
#include "ShaderManager.hpp"
#include "Parsers/SceneParser.hpp"
#include "Components/Collider.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"

namespace EVA
{
	Scene::Scene()
	{
		/*m_ShadowMaterial.shader = ShaderManager::CreateOrGetShader("scene_shadow", "shadow.vert", "shadow.frag");
		m_ShadowMaterialInstanced.shader = ShaderManager::CreateOrGetShader("scene_shadow_instanced", "shadow_instanced.vert", "shadow.frag");
	
		m_ShadowMaterialCube.shader = ShaderManager::CreateOrGetShader("scene_shadow_cube", "shadow_cube.vert", "shadow_cube.frag", "shadow_cube.geom");
		m_ShadowMaterialCubeInstanced.shader = ShaderManager::CreateOrGetShader("scene_shadow_cube_instanced", "shadow_cube_instanced.vert", "shadow_cube.frag", "shadow_cube.geom");*/

		m_ShadowMaterial.shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "shadow.shader");
		m_ShadowMaterialInstanced.shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "shadow_instaced.shader");

		m_ShadowMaterialCube.shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "shadow_cube.shader");
		m_ShadowMaterialCubeInstanced.shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "shadow_cube_instanced.shader");
	}

	Scene::Scene(const FS::path& path) : Scene()
	{
		SceneParser::Load(this, path);
	}

	Scene::~Scene()
	{
		std::cout << "Scene::~Scene() - Deconstructing \n";
	}

	void Scene::Awake()
	{
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Awake();
		}
	}

	void Scene::Start()
	{
		m_Started = true;

		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Start();
		}
	}

	void Scene::Update(const float deltaTime)
	{
		// Update
		for (auto &gameObject : m_GameObjects)
		{
			if (!gameObject->active)
				continue;;

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
			if (!gameObject->active)
				continue;

			gameObject->LateUpdate();
		}

		// Destroy
		ProcessDestroyQueue();
	}

	void Scene::Render()
	{

		// Shadows 
		glDisable(GL_CULL_FACE);
		for (auto& light : m_Lights)
		{
			if (light->GetType() == Light::Directional && light->Shadows())
			{
				glViewport(0, 0, light->GetShadwoSize(), light->GetShadwoSize());
				glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthMapFb());
				glClear(GL_DEPTH_BUFFER_BIT);

				RenderShadowMap(light->GetLightSpaceMatrix());

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else if (light->GetType() == Light::Point && light->Shadows())
			{
				glViewport(0, 0, light->GetShadwoSize(), light->GetShadwoSize());
				glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthMapFb());
				glClear(GL_DEPTH_BUFFER_BIT);
				
				RenderShadowCubeMap(light->GetShadowTransforms(), light->position, light->pointFarPlane);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}
		glEnable(GL_CULL_FACE);

		// Scene
		RenderScene();

		// Render components
		for (auto &gameObject : m_GameObjects)
		{
			gameObject->Render();
		}

		// UI
		RenderUi();

		// Reset the active material
		Material::SetNoActive();
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

	void Scene::RemoveFromNameMap(GameObject* gameObject)
	{
		const auto iterator = m_NameMap.find(gameObject->GetName());

		if (iterator != m_NameMap.end())
			m_NameMap.erase(iterator);
	}

	GameObject* Scene::FindGameObjectByName(const std::string& name)
	{
		const auto iterator = m_NameMap.find(name);

		if (iterator == m_NameMap.end())
			return nullptr;

		return m_NameMap[name];
	}

	std::string Scene::AddToNameMap(GameObject* gameObject)
	{
		auto iterator = m_NameMap.find(gameObject->GetName());

		if (iterator == m_NameMap.end())
		{
			m_NameMap[gameObject->GetName()] = gameObject;
			return gameObject->GetName();
		}

		do
		{
			m_NameCounter++;
			iterator = m_NameMap.find(gameObject->GetName() + std::to_string(m_NameCounter));
		}
		while (iterator != m_NameMap.end());

		return gameObject->GetName() + std::to_string(m_NameCounter);
	}

	std::shared_ptr<Light> Scene::CreateLight(Light::Type type, const bool shadows, const unsigned int shadowSize)
	{
		auto light = std::make_shared<Light>(type, shadows, shadowSize);

		m_Lights.push_back(light);

		return light;
	}

	Light* Scene::CreateLight(DataObject data)
	{
		auto light = std::make_shared<Light>(data);

		m_Lights.push_back(light);

		return light.get();
	}

	void Scene::DestroyLight(Light* light)
	{
		for (unsigned int i = 0; i < m_Lights.size(); ++i)
		{
			if (m_Lights[i].get() == light)
			{
				m_Lights.erase(m_Lights.begin() + i);
				break;
			}
		}
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

	void Scene::AddCollider(Collider* collider)
	{
		for (auto& c : m_Colliders)
		{
			if (c == collider)
			{
				return;
			}
		}

		m_Colliders.push_back(collider);
	}

	void Scene::RemoveCollider(Collider* collider)
	{
		for (unsigned int i = 0; i < m_Colliders.size(); ++i)
		{
			if(m_Colliders[i] == collider)
			{
				m_Colliders.erase(m_Colliders.begin() + i);
				return;
			}
		}
	}

	std::vector<Collider*>& Scene::GetColliders()
	{
		return m_Colliders;
	}

	void Scene::RenderScene()
	{
		const auto ws = Application::GetWindowSize();
		glViewport(0, 0, ws.x, ws.y);

		if (skybox != nullptr)
		{
			skybox->Render();
		}

		// For each material
		for (auto &material : m_Materials)
		{
			// If the material should use GPU instancing
			if (material[0][0]->material->useInstancing)
			{
				// Set material / shader
				material[0][0]->material->Activate(this, nullptr);

				// For each mesh
				for (auto &meshes : material)
				{
					// If missing mbo or the mesh/material combo is marked dirty
					if (!meshes[0]->material->HasMbo(meshes[0]->mesh) || meshes[0]->material->IsDirty(meshes[0]->mesh))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->transform->modelMatrix);
						}

						// Set the mbo
						meshes[0]->material->SetMbo(meshes[0]->mesh, models);

						// Set not dirty
						meshes[0]->material->SetDirty(meshes[0]->mesh, false);
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced(meshes[0]->material->GetMbo(meshes[0]->mesh));
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
						if(!meshRenderer->active)
							continue;

						// Render the mesh at the MeshRenderers position
						meshRenderer->Render();
					}
				}
			}
		}
	}

	void Scene::RenderShadowMap(const glm::mat4 lightSpaceMatrix)
	{
		if(m_ShadowMaterialInstanced.shader == nullptr || m_ShadowMaterial.shader == nullptr)
			return;

		for (auto &material : m_Materials)
		{
			if(!material[0][0]->material->castShadows)
				continue;

			// If the material should use GPU instancing
			if (material[0][0]->material->useInstancing)
			{
				// Set material / shader
				m_ShadowMaterialInstanced.Activate(this, nullptr);
				m_ShadowMaterialInstanced.shader->SetUniformMatrix4Fv("lightSpaceMatrix", lightSpaceMatrix);

				// For each mesh
				for (auto &meshes : material)
				{
					// If missing mbo or the mesh/material combo is marked dirty
					if (!meshes[0]->material->HasMbo(meshes[0]->mesh) || meshes[0]->material->IsDirty(meshes[0]->mesh))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->transform->modelMatrix);
						}

						// Set the mbo
						meshes[0]->material->SetMbo(meshes[0]->mesh, models);

						// Set not dirty
						meshes[0]->material->SetDirty(meshes[0]->mesh, false);
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced(meshes[0]->material->GetMbo(meshes[0]->mesh));
				}
			}
			else // If not
			{
				m_ShadowMaterial.Activate(this, nullptr);
				m_ShadowMaterial.shader->SetUniformMatrix4Fv("lightSpaceMatrix", lightSpaceMatrix);

				// For each mesh
				for (auto &meshes : material)
				{
					// For each MeshRenderer that use the mesh
					for (auto &meshRenderer : meshes)
					{
						if (!meshRenderer->active)
							continue;

						// Render the mesh at the MeshRenderers position
						m_ShadowMaterial.SetObjectUniforms(meshRenderer->transform.Get());
						meshRenderer->mesh->Draw();
					}
				}
			}
		}
	}

	void Scene::RenderShadowCubeMap(const std::vector<glm::mat4>& shadowMatrices, const glm::vec3 lightPos, const float farPlane)
	{
		if (m_ShadowMaterialCubeInstanced.shader == nullptr || m_ShadowMaterialCube.shader == nullptr)
			return;

		for (auto &material : m_Materials)
		{
			if (!material[0][0]->material->castShadows)
				continue;

			// If the material should use GPU instancing
			if (material[0][0]->material->useInstancing)
			{
				// Set material / shader
				m_ShadowMaterialCubeInstanced.Activate(this, nullptr);
				m_ShadowMaterialCubeInstanced.shader->SetUniform3Fv("lightPos", lightPos);
				m_ShadowMaterialCubeInstanced.shader->SetUniform1F("farPlane", farPlane);
				for (unsigned int i = 0; i < shadowMatrices.size(); ++i)
				{
					m_ShadowMaterialCubeInstanced.shader->SetUniformMatrix4Fv("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
				}

				// For each mesh
				for (auto &meshes : material)
				{
					// If missing mbo or the mesh/material combo is marked dirty
					if (!meshes[0]->material->HasMbo(meshes[0]->mesh) || meshes[0]->material->IsDirty(meshes[0]->mesh))
					{
						// Get the model matrices from all the objects
						std::vector<glm::mat4> models;
						models.reserve(meshes.size());

						for (auto &meshRenderer : meshes)
						{
							models.push_back(meshRenderer->transform->modelMatrix);
						}

						// Set the mbo
						meshes[0]->material->SetMbo(meshes[0]->mesh, models);

						// Set not dirty
						meshes[0]->material->SetDirty(meshes[0]->mesh, false);
					}

					// Draw the mesh
					meshes[0]->mesh->DrawInstanced(meshes[0]->material->GetMbo(meshes[0]->mesh));
				}
			}
			else // If not
			{
				m_ShadowMaterialCube.Activate(this, nullptr);
				m_ShadowMaterialCube.shader->SetUniform3Fv("lightPos", lightPos);
				m_ShadowMaterialCube.shader->SetUniform1F("farPlane", farPlane);
				for (unsigned int i = 0; i < shadowMatrices.size(); ++i)
				{
					m_ShadowMaterialCube.shader->SetUniformMatrix4Fv("shadowMatrices[" + std::to_string(i) + "]", shadowMatrices[i]);
				}

				// For each mesh
				for (auto &meshes : material)
				{
					// For each MeshRenderer that use the mesh
					for (auto &meshRenderer : meshes)
					{
						if (!meshRenderer->active)
							continue;

						// Render the mesh at the MeshRenderers position
						m_ShadowMaterialCube.SetObjectUniforms(meshRenderer->transform.Get());
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
					for (auto& child : m_GameObjects[i]->transform->GetChildren())
					{
						child->SetParent((Transform*)nullptr);
					}

					m_GameObjects[i]->transform->SetParent((Transform*)nullptr);

					m_GameObjects.erase(m_GameObjects.begin() + i);
					break;
				}
			}
		}
		m_DestroyQueue.clear();
	}
}
