#pragma once

#include "glm/glm.hpp"
#include <vector>
#include "Parsers/Json.hpp"

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
		glm::vec2 m_Rotation;

		// Point light
		glm::vec3 m_Position;

		// Shadows
		bool m_Shadows;
		unsigned int m_DepthMapFb{};
		unsigned int m_DepthMap{};
		unsigned int m_ShadowMapSize;

	public:
		
		static const unsigned int DEFAULT_SHADOW_MAP_SIZE = 4096;
		
		float directionalShadowDistance = 20.0f;

		float directionalNearPlane = 1.0f;
		float directionalFarPlane = 100.0f;

		float pointNearPlane = 0.1f;
		float pointFarPlane = 25.0f;

		// Common
		glm::vec3 color = glm::vec3(1.0f);
		float ambientCoefficient = 0.005f;

		// Point light
		const glm::vec3& position = m_Position;
		float attenuation = 0.2f;

		// Constructor
		explicit Light(LightType type, bool shadows, unsigned int shadowSize = DEFAULT_SHADOW_MAP_SIZE);
		explicit Light(DataObject data);

		void SetPosition(glm::vec3 newPosition);
		void SetRotation(glm::vec2 rotation);

		// Get stuff
		inline LightType GetType()		const { return m_Type; }
		inline glm::vec4 GetDirection() const { return m_Direction; }

		glm::mat4 GetLightSpaceMatrix() const;
		inline bool Shadows() const { return m_Shadows; }
		inline unsigned int GetDepthMapFb() const { return m_DepthMapFb; }
		inline unsigned int GetDepthMap() const { return m_DepthMap; }
		inline unsigned int GetShadwoSize() const { return m_ShadowMapSize; }

		std::vector<glm::mat4> GetShadowTransforms() const;
	};

}
