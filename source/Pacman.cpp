#include "Pacman.hpp"

Pacman::Pacman(EVA::GameObject* gameObject) : Component(gameObject)
{
	m_Mesh = EVA::Mesh::Load("./assets/models/pacman.obj");

	m_Material = std::make_shared<EVA::Material>();
	m_Material->shader = std::make_shared<EVA::StandardShader>();
	m_Material->SetTexture(EVA::TextureType::Diffuse, "./assets/textures/pacman/pacman_diffuse.png");
	m_Material->SetTexture(EVA::TextureType::Specular, "./assets/textures/pacman/pacman_specular.png");
	m_Material->SetTexture(EVA::TextureType::Normal, "./assets/textures/pacman/pacman_normal.png");

	auto mr = m_GameObject->AddComponent<EVA::MeshRenderer>();
	mr->Set(m_Mesh, m_Material);
}