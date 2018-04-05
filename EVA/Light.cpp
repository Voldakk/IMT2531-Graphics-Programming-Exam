#include "Light.hpp"

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.inl"
#include "Application.hpp"

EVA::Light::Light(const LightType type, const bool shadows, const unsigned int shadowSize)
{
	m_Type = type;
	m_Shadows = shadows;
	m_ShadowSize = shadowSize;

	if(!shadows)
		return;;

	if(m_Type == LightType::Directional)
	{
		glGenTextures(1, &m_DepthMap);
		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			m_ShadowSize, m_ShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glGenFramebuffers(1, &m_DepthMapFb);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glGenTextures(1, &m_DepthMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthMap);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				m_ShadowSize, m_ShadowSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		glGenFramebuffers(1, &m_DepthMapFb);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFb);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void EVA::Light::SetRotation(const glm::vec2 rotation)
{
	m_Rotation = rotation;

	const auto pitch = glm::radians(rotation.x);
	const auto yaw = glm::radians(rotation.y);

    m_Direction.x = std::cos(yaw) * std::cos(pitch);
    m_Direction.y = std::sin(pitch);
    m_Direction.z = std::sin(yaw) * std::cos(pitch);
    m_Direction.w = 0.0f;
}

glm::mat4 EVA::Light::GetLightSpaceMatrix() const
{
	const auto lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, directionalNearPlane, directionalFarPlane);

	const auto lightDirection = -glm::normalize(glm::vec3(GetDirection()));
	const auto cameraPosition = Application::mainCamera->GetGameObject()->GetTransform()->position;


	const auto lightView = glm::lookAt(
		glm::vec3(cameraPosition - lightDirection * (directionalFarPlane / 2)),
		glm::vec3(cameraPosition),
		glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

std::vector<glm::mat4> EVA::Light::GetShadowTransforms() const
{
	const auto shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, pointNearPlane, pointFarPlane);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return shadowTransforms;
}
