#pragma once

#include "EVA.hpp"

#include "Components/Terrain.hpp"
#include "Components/EnviromentManager.hpp"

class TerrainMaterial : public EVA::Material
{
	Terrain* m_Terrain;
	EnviromentManager * m_EnviromentManager;

public:


	TerrainMaterial(Terrain* terrain, EnviromentManager * enviromentManager)
		: m_Terrain(terrain), m_EnviromentManager(enviromentManager)
	{
		
	}

	void SetMaterialUniforms(EVA::Scene* scene) const override
	{
		Material::SetMaterialUniforms(scene);
		

		shader->SetUniform1F("maxTerrainHeight", m_Terrain->maxTerrainHeight);
		shader->SetUniform1I("numRegions", m_EnviromentManager->regions.size());
		shader->SetUniform1F("regionBlendAmount", m_EnviromentManager->regionBlendAmount);

		for (unsigned int i = 0; i < m_EnviromentManager->regions.size(); ++i)
		{
			const auto regionNum = "regions[" + std::to_string(i) + "].";

			shader->SetUniform1F(regionNum + "minHeight", m_EnviromentManager->regions[i].minHeight);
			shader->SetUniform1F(regionNum + "maxHeight", m_EnviromentManager->regions[i].maxHeight);
			shader->SetUniform3Fv(regionNum + "color", m_EnviromentManager->regions[i].color);
		}
	}
};