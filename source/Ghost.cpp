#include "Ghost.hpp"

Ghost::Ghost(EVA::GameObject* gameObject, const glm::vec3 color) : Component(gameObject)
{
	const auto shader = std::make_shared<EVA::StandardShader>();

	m_Mesh = EVA::Mesh::LoadMultiple("./assets/models/ghost.obj");

	// Body
	m_BodyMaterial= std::make_shared<EVA::Material>();
	m_BodyMaterial->shader = shader;
	m_BodyMaterial->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/ghost/ghost_body_diffuse.png");
	m_BodyMaterial->SetTexture(EVA::TextureType::Specular, "./assets/textures/ghost/ghost_body_specular.png");
	m_BodyMaterial->SetTexture(EVA::TextureType::Normal, "./assets/textures/ghost/ghost_body_normal.png");

	m_BodyMaterial->tintDiffuse = glm::vec4(color, 1.0f);

	auto bodyMr = gameObject->AddComponent<EVA::MeshRenderer>();
	bodyMr->Set(m_Mesh[1], m_BodyMaterial);

	// Eyes
	m_EyeMaterial = std::make_shared<EVA::Material>();
	m_EyeMaterial->shader = shader;
	m_EyeMaterial->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/ghost/ghost_eyes_diffuse.png");
	m_EyeMaterial->SetTexture(EVA::TextureType::Specular, "./assets/textures/ghost/ghost_eyes_specular.png");
	m_EyeMaterial->SetTexture(EVA::TextureType::Normal, "./assets/textures/ghost/ghost_eyes_normal.png");

	auto eyeMr = gameObject->AddComponent<EVA::MeshRenderer>();
	eyeMr->Set(m_Mesh[0], m_EyeMaterial);
}