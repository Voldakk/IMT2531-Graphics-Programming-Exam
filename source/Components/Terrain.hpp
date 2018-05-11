#pragma once

#include <memory>

#include "EVA.hpp"
#include "EVA/ResourceManagers.hpp"

class Terrain : public EVA::Component
{
	REGISTER_COMPONENT_HPP(Terrain);

	FS::path m_HeightMapPath;

	std::vector<std::vector<float>> m_HeightData;

	EVA::MeshRenderer* m_MeshRenderer = nullptr;

public:

	void Start() override;
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;

private:
	
	void SetHeightMap(const FS::path& newHeightMapPath);
	void CreateMesh();

	float HeightData(float x, float y);
};
