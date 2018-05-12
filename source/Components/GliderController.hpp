#pragma once

#include "EVA.hpp"
#include "EVA/UI.hpp"

class GilderController : public EVA::Component, public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(GilderController)

	float m_MinSpeed = 1.0f;
	float m_MaxSpeed = 10.0f;
	float m_CurrentSpeed = m_MinSpeed;

	float m_AccelerationTime = 3.0f;

	float m_PitchSpeed = 45.0f;
	float m_YawSpeed = 45.0f;
	float m_RollSpeed = 45.0f;

	EVA::Label* m_SpeedLabel = nullptr;

	glm::vec3 m_StartLocation;
	glm::quat m_StartOrientation;

public:

	void Start() override;
	
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;
	
	void Update(float deltaTime) override;
};