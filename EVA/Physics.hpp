#pragma once

#include "glm/glm.hpp"

#include "Application.hpp"
#include "GameObject.hpp"
#include "Components/Camera.hpp"

namespace EVA
{

	struct Ray
	{
		glm::vec3 origin;
		glm::vec3 direction;
	};

	struct RaycastHit
	{
		Ray ray;
		glm::vec3 point;
		float distance = 0.0f;
		GameObject* hitObject = nullptr;

		RaycastHit() = default;

		RaycastHit(const Ray ray, const glm::vec3 point, const float distance, GameObject* hitObject)
		{
			this->ray = ray;
			this->point = point;
			this->distance = distance;
			this->hitObject = hitObject;

		}
	};

	class Physics
	{
	public:

		/**
		 * \brief Creates a ray from the mouse position pointing into the screen
		 * \param mousePos Mouse position, in pixels, from top-left corner of the window
		 * \param camera Camera position and orientation
		 * \return The ray
		 */
		static Ray ScreenPosToWorldRay(glm::ivec2 mousePos, const EVA::Camera* camera);

		/**
		 * \brief Tests for intersections between the ray and the aabb
		 * \param ray The ray
		 * \param aabbMin Minimum X,Y,Z coords of the mesh when not transformed at all.
		 * \param aabbMax Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
		 * \param modelMatrix Transformation applied to the mesh (which will thus be also applied to its bounding box)
		 * \param intersectionDistance Output : distance between ray_origin and the intersection with the OBB
		 * \return Whether the ray hit an object
		 */
		static bool TestRayObbIntersection(Ray ray, glm::vec3 aabbMin, glm::vec3 aabbMax, glm::mat4 modelMatrix, float& intersectionDistance);

		static bool Raycast(Ray ray, Scene* scene, RaycastHit& out);

	};

}
