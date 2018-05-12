#include "CameraController.hpp"

#include "EVA/Input.hpp"

REGISTER_COMPONENT_CPP(CameraController, "CameraController")

void CameraController::Start()
{
	m_Camera = gameObject->GetComponentOfType<EVA::Camera>();

	m_FreeLook = gameObject->GetComponentOfType<EVA::FreeLook>();
	m_FollowTarget = gameObject->GetComponentOfType<EVA::FollowTarget>();

	auto go = scene->FindGameObjectByName("FPP Camera");
	if (go != nullptr)
		m_FppCamera = go->GetComponentOfType<EVA::Camera>();

	UpdateCameras();
}

void CameraController::LateUpdate()
{
	if(EVA::Input::KeyDown(EVA::Input::Slash))
	{
		m_CurrentCamera++;
		if (m_CurrentCamera == Last)
			m_CurrentCamera = 0;
	}
}

void CameraController::UpdateCameras()
{
	switch (m_CurrentCamera)
	{
	case Fpp:
		m_FreeLook->SetActive(false);
		m_FollowTarget->SetActive(false);
		m_FppCamera->SetActive(true);
		m_Camera->SetActive(false);
		break;
	case FreeLook:
		m_FreeLook->SetActive(true);
		m_FollowTarget->SetActive(false);
		m_FppCamera->SetActive(false);
		m_Camera->SetActive(true);
		break;
	case Follow:
	default:
		m_FreeLook->SetActive(false);
		m_FollowTarget->SetActive(true);
		m_FppCamera->SetActive(false);
		m_Camera->SetActive(true);
		break;;
	}
}
