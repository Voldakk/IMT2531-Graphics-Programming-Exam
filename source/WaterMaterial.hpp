#pragma once

#include "EVA.hpp"

class WaterMaterial : public EVA::Material
{ 
	Water* m_Water;

public:

	explicit WaterMaterial(Water* water)
		: m_Water(water)
	{

	}

	void SetMaterialUniforms(EVA::Scene* scene) const override
	{
		Material::SetMaterialUniforms(scene);

		shader->SetUniform1F("time", m_Water->time);

		shader->SetUniform1F("waveLength", m_Water->waveLength);
		shader->SetUniform1F("amplitude", m_Water->amplitude);
		shader->SetUniform1F("verticesPerUnit", m_Water->verticesPerUnit);
	}
};