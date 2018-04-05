#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace EVA
{
	

	enum LightType
	{
		Directional, Point
	};

	class Light
	{

		LightType m_Type;

		// Directional light
		glm::vec4 m_Direction;
		glm::vec2 m_Rotation = { 30.0f, 0.0f };

		// Shadows
		bool m_Shadows;
		unsigned int m_DepthMapFb{};
		unsigned int m_DepthMap{};
		unsigned int m_ShadowSize;


	public:

		const float directionalNearPlane = 1.0f;
		const float directionalFarPlane = 100.0f;

		const float pointNearPlane = 1.0f;
		const float pointFarPlane = 25.0f;

		// Common
		glm::vec3 color = glm::vec3(1.0f);
		float ambientCoefficient = 0.005f;

		// Point light
		glm::vec3 position;
		float attenuation = 0.2f;

		// Constructor
		explicit Light(LightType type, bool shadows, unsigned int shadowSize);

		void SetRotation(glm::vec2 rotation);

		// Get stuff
		inline LightType GetType()		const { return m_Type; }
		inline glm::vec4 GetDirection() const { return m_Direction; }

		glm::mat4 GetLightSpaceMatrix() const;
		inline bool Shadows() const { return m_Shadows; }
		inline unsigned int GetDepthMapFb() const { return m_DepthMapFb; }
		inline unsigned int GetDepthMap() const { return m_DepthMap; }
		inline float GetShadwoSize() const { return m_ShadowSize; }

		std::vector<glm::mat4> GetShadowTransforms() const;
	};

}
