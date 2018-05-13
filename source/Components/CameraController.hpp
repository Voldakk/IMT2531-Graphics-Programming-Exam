#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"

class CameraController : public EVA::Component, public EVA::IUpdateComponent, public EVA::ILateUpdateComponent
{

	REGISTER_COMPONENT_HPP(CameraController);

	EVA::Camera* m_Camera = nullptr;

	EVA::FreeLook* m_FreeLook = nullptr;
	EVA::FollowTarget* m_FollowTarget = nullptr;
	EVA::Camera* m_FppCamera = nullptr;

	enum CameraType { FreeLook, Follow, Fpp, Last };
	int m_CurrentCamera = 0;

public:

	void Start() override;
	void LateUpdate() override;

	void Update(float deltaTime) override;
	void UpdateCameras();
};