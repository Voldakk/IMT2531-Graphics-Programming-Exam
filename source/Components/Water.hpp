#pragma once

#include "EVA.hpp"
#include "Terrain.hpp"

class Water : public EVA::Component , public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(Water)
	
	struct Properties
	{
		float height = 0.0f;

		float waveLength = 1.0f;
		float amplitude = 1.0f;

		float materialShininess = 1.0f;
		float specularStrength = 1.0f;

		glm::vec4 color = glm::vec4(1.0f);
	};

	Terrain* m_Terrain = nullptr;
	EnviromentManager* m_EnviromentManager = nullptr;
	EVA::MeshRenderer* m_MeshRenderer = nullptr;

	float m_VerticesPerUnit = 1.0f;
	float m_TimeScale = 1.0f;

	std::shared_ptr<EVA::Material> m_Material;

public:

	const float& verticesPerUnit = m_VerticesPerUnit;
	float time = 0.0f;

	Properties summerProperties;
	Properties winterProperties;
	
	void Start() override;

	void Update(float deltaTime) override;

	void GenerateMesh() const;

	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;

	Properties GetWaterProperties() const;
};
