#pragma once

#include "EVA.hpp"
#include "EVA/UI.hpp"

class GilderController : public EVA::Component, public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(GilderController)

	float m_MinSpeed = 10.0f;
	float m_MaxSpeed = 100.0f;
	float m_CurrentSpeed = m_MinSpeed;

	float m_AccelerationTime = 3.0f;

	float m_PitchSpeed = 45.0f;
	float m_YawSpeed = 45.0f;
	float m_RollSpeed = 90.0f;

	EVA::Label* m_ThrottleLabel = nullptr;
	EVA::Label* m_SpeedLabel = nullptr;

	glm::vec3 m_StartLocation;
	glm::quat m_StartOrientation;

	std::vector<EVA::Transform*> m_TeleportPoints;
	unsigned int m_CurrentTeleportPoint = 0;

	// Glider parts
	EVA::Transform* m_Propeller = nullptr;

	EVA::Transform* m_AileronLeft = nullptr;
	EVA::Transform* m_AileronRight = nullptr;

	EVA::Transform* m_Profondeur = nullptr;
	EVA::Transform* m_Direction = nullptr;

	EVA::Transform* m_Stick = nullptr;
	EVA::Transform* m_Throttle = nullptr;
	float m_MaxThrottlePositionZ = 0.0f;

	glm::vec3 m_Velocity;

public:

	void Start() override;
	void Update(float deltaTime) override;
	
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;
};