#pragma once

#include "EVA.hpp"

class GilderController : public EVA::Component, public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(GilderController)

	float m_MinSpeed = 1.0f;
	float m_MaxSpeed = 10.0f;
	float m_CurrentSpeed = m_MinSpeed;

	float m_PitchSpeed = 45.0f;
	float m_YawSpeed = 45.0f;

	float m_Pitch = 0.0f;
	float m_Yaw = 0.0f;
	float m_Roll = 0.0f;

public:

	void Start() override;
	
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;
	
	void Update(float deltaTime) override;
};