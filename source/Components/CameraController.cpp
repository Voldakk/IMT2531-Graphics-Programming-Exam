#include "CameraController.hpp"

#include "EVA/Input.hpp"

REGISTER_COMPONENT_CPP(CameraController, "CameraController")

void CameraController::Start()
{
	m_Camera = gameObject->GetComponentOfType<EVA::Camera>();

	m_FreeLook = gameObject->GetComponentOfType<EVA::FreeLook>();
	m_FollowTarget = gameObject->GetComponentOfType<EVA::FollowTarget>();

	m_FppCamera = scene->FindGameObjectByName("FPP Camera")->GetComponentOfType<EVA::Camera>();
	m_TailCamera = scene->FindGameObjectByName("Tail Camera")->GetComponentOfType<EVA::Camera>();
	m_WingCamera = scene->FindGameObjectByName("Wing Camera")->GetComponentOfType<EVA::Camera>();

	m_CurrentCamera = Follow;

	UpdateCameras();
}

void CameraController::Update(const float deltaTime)
{
	if (EVA::Input::Key(EVA::Input::M))
	{
		m_Camera->fov += deltaTime * 20.0f;
	}
	if (EVA::Input::Key(EVA::Input::N))
	{
		m_Camera->fov -= deltaTime * 20.0f;
	}

	m_Camera->fov = glm::clamp(m_Camera->fov, 30.0f, 120.0f);

	if (EVA::Input::KeyDown(EVA::Input::Slash))
	{
		m_CurrentCamera++;
		if (m_CurrentCamera == Last)
			m_CurrentCamera = 0;

		UpdateCameras();
	}
}

void CameraController::LateUpdate()
{
	
}

void CameraController::UpdateCameras() const
{
	DeactivateAll();

	switch (m_CurrentCamera)
	{
	case Fpp:
		m_FppCamera->SetActive(true);
		break;
	case Tail:
		m_TailCamera->SetActive(true);
		break;
	case Wing:
		m_WingCamera->SetActive(true);
		break;
	case FreeLook:
		m_Camera->SetActive(true);
		m_FreeLook->SetActive(true);
		break;
	case Follow:
	default:
		m_Camera->SetActive(true);
		m_FollowTarget->SetActive(true);
		break;;
	}
}

void CameraController::DeactivateAll() const
{
	m_Camera->SetActive(false);
	m_FreeLook->SetActive(false);
	m_FollowTarget->SetActive(false);
	m_FppCamera->SetActive(false);
	m_TailCamera->SetActive(false);
	m_WingCamera->SetActive(false);
}
