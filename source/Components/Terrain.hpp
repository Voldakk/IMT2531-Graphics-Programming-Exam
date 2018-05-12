#pragma once

#include <memory>

#include "EVA.hpp"
#include "EVA/ResourceManagers.hpp"

#include "EnviromentManager.hpp"

class Terrain : public EVA::Component, public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(Terrain)

	FS::path m_HeightMapPath;

	std::vector<std::vector<float>> m_HeightData;

	EVA::MeshRenderer* m_MeshRenderer = nullptr;
	EnviromentManager* m_EnviromentManager = nullptr;

	std::shared_ptr<EVA::Material> m_Material;

	float m_VerticesPerUnit = 1.0f;
	float m_TerrainWidth = 100.0f;
	float m_TerrainLength;
	float m_MaxTerrainHeight = 40.0f;

	float m_MaterialShininess = 32;
	float m_SpecularStrength = 1.0f;

public:

	const float& terrainWidth = m_TerrainWidth;
	const float& terrainLength = m_TerrainLength;

	const float& maxTerrainHeight = m_MaxTerrainHeight;
	const float& specularStrength = m_SpecularStrength;
	bool contourLines = false;

	void Start() override;
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;
	
	void Update(float deltaTime) override;
	
	float HeightData(float x, float y);

private:
	
	void SetHeightMap(const FS::path& newHeightMapPath);
	void CreateMesh();

};
