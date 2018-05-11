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


public:

	bool contourLines = false;
	float maxTerrainHeight = 40.0f;

	void Start() override;
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;
	
	void Update(float deltaTime) override;

private:
	
	void SetHeightMap(const FS::path& newHeightMapPath);
	void CreateMesh();

	float HeightData(float x, float y);
};
