#include "Scene.hpp"

#include "GL/glew.h"

#include "Application.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace EVA
{
	unsigned int depthMapFBO;

	const unsigned int SHADOW_SIZE = 4096;

	unsigned int depthMap;

	Material shadowMaterial;
	Material shadowMaterialInstanced;

	Scene::Scene()
	{
		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		const auto nearPlane = 1.0f;
		const auto farPlane = 100.0f;
		const auto lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, nearPlane, farPlane);

		const auto lightDirection = -glm::normalize(glm::vec3(m_lights[0]->GetDirection()));
		const auto cameraPosition = Application::mainCamera->GetGameObject()->GetTransform()->position;
		

		const auto lightView = glm::lookAt(
			glm::vec3(cameraPosition - lightDirection * (farPlane/2)),
			glm::vec3(cameraPosition),
			glm::vec3(0.0f, 1.0f, 0.0f));

		const auto lightSpaceMatrix = lightProjection * lightView;

		// Shadow map
		glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderShadowMap(lightSpaceMatrix);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Scene
		const auto ws = Application::GetWindowSize();
		glViewport(0, 0, ws.x, ws.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(lightSpaceMatrix);
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
				materials[0][0]->GetMaterial()->SetTexture(TextureType::ShadowMap, depthMap);
				materials[0][0]->GetMaterial()->Activate(this, nullptr);
				materials[0][0]->GetMaterial()->shader->SetUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);

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
						meshRenderer->GetMaterial()->SetTexture(TextureType::ShadowMap, depthMap);
						meshRenderer->GetMaterial()->Activate(this, meshRenderer->GetGameObject()->GetTransform().get());
						meshRenderer->GetMaterial()->shader->SetUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);
						meshRenderer->GetMesh()->Draw();
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
