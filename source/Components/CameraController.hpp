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
	EVA::Camera* m_TailCamera = nullptr;
	EVA::Camera* m_WingCamera = nullptr;

	enum CameraType { Fpp, Tail, Wing, Follow, FreeLook, Last };
	int m_CurrentCamera = 0;

public:

	void Start() override;
	void LateUpdate() override;

	void Update(float deltaTime) override;
	void UpdateCameras() const;

	void DeactivateAll() const;
};