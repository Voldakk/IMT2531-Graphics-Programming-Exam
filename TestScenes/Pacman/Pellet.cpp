#include "Pellet.hpp"

#include "EVA/ResourceManagers.hpp"
#include "EVA/Input.hpp"

#include "Game.hpp"

std::shared_ptr<EVA::Mesh> Pellet::m_Mesh;
std::shared_ptr<EVA::Material> Pellet::m_Material;

unsigned int Pellet::m_PelletCount;

void Pellet::Start()
{
	if (m_Mesh == nullptr)
	{
		m_Mesh = EVA::ModelManager::Primitive(EVA::ModelManager::Sphere)->GetMesh(0);
		//m_Mesh->SetStatic(true);

		m_Material = std::make_shared<EVA::Material>();
		//m_Material->enableInstancing = true;
		m_Material->tintDiffuse = { 1.0f, 1.0f, 0.0f, 1.0f };
		m_Material->shader = EVA::ShaderManager::GetShader("standard_instanced");
	}

	auto mr = gameObject->AddComponent<EVA::MeshRenderer>();
	mr->Set(m_Mesh, m_Material);
	//m_Mesh->isDirty = true;

	transform->SetScale(0.1f);
	transform->SetPosition(TileMap::GetTilePosition(tile) + EVA::YAXIS * transform->scale.y);

	m_PelletCount++;
}

void Pellet::Update(const float deltaTime)
{
	if (game->pacman->currentTile == tile)
	{
		OnPickup();
		gameObject->Destroy();
		//m_Mesh->isDirty = true;
	}
}

void Pellet::OnPickup()
{
	game->AddScore(m_Score);

	m_PelletCount--;
	if (m_PelletCount == 0)
		game->Win();
}

void Pellet::ResetCount()
{
	m_PelletCount = 0;
}

void Energizer::Start()
{
	Pellet::Start();

	Pellet::m_Score = m_Score;

	transform->SetScale(0.3f);
}

void Energizer::OnPickup()
{
	Pellet::OnPickup();
	game->ActivateEnergizer(m_Time);
}
