#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"

class CameraController : public EVA::Component, public EVA::IUpdateComponent
{

	REGISTER_COMPONENT_HPP(CameraController);

	EVA::FreeLook* m_FreeLook = nullptr;
	EVA::FollowTarget* m_FollowTarget = nullptr;

	enum CameraType { FreeLook, Follow, Last };
	int m_CurrentCamera = 0;

public:

	void Start() override;
	void Update(float deltaTime) override;
};