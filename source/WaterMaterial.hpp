#pragma once

#include "EVA.hpp"

class WaterMaterial : public EVA::Material
{ 
	Water* m_Water;

public:

	explicit WaterMaterial(Water* water)
		: m_Water(water)
	{
		castShadows = false;
	}

	void SetMaterialUniforms(EVA::Scene* scene) const override
	{
		Material::SetMaterialUniforms(scene);

		const auto p = m_Water->GetWaterProperties();

		shader->SetUniform1F("time", m_Water->time);
		shader->SetUniform1F("verticesPerUnit", m_Water->verticesPerUnit);
		
		shader->SetUniform1F("waveLength", p.waveLength);
		shader->SetUniform1F("amplitude", p.amplitude);

		shader->SetUniform1F("specularStrength", p.specularStrength);
		shader->SetUniform1F("material.shininess", p.materialShininess);

		shader->SetUniform4Fv("material.tint_diffuse", p.color);
	}
};