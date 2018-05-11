#pragma once

#include "EVA.hpp"

class EnviromentManager : public EVA::Component, public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(EnviromentManager)

	EVA::Light* m_Sun = nullptr;
	
	float m_Season = 0.0f;
	float m_Time = 0.0f;
	float m_SecondsPerDay = 10.0f;

	float m_MiddayAngle = 60.0f;

	glm::vec3 m_SunriseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	glm::vec3 m_MiddayColor = glm::vec3(1.0f);
	glm::vec3 m_SunsetColor = glm::vec3(1.0f, 0.5f, 0.5f);
	glm::vec3 m_NightColor = glm::vec3(0.01f);

public:

	void Awake() override;
	
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;

	void Update(float deltaTime) override;

	void UpdateTime() const;
};