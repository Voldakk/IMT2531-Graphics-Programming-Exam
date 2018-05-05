#pragma once

#include "glm/glm.hpp"

#include "Application.hpp"
#include "GameObject.hpp"
#include "Components/Camera.hpp"

namespace EVA
{

	/**
	 * \brief A ray for use in ray casting
	 */
	struct Ray
	{
		glm::vec3 origin;
		glm::vec3 direction;

		/**
		* \brief Default constructor
		*/
		Ray() = default;

		/**
		 * \brief 
		 * \param origin The ray's origin
		 * \param direction The ray's direction
		 */
		Ray(const glm::vec3 origin, const glm::vec3 direction)
		{
			this->origin = origin;
			this->direction = direction;
		}
	};

	/**
	 * \brief Holds information about a raycast hit
	 */
	struct RaycastHit
	{
		Ray ray;
		glm::vec3 point;
		float distance = 0.0f;
		GameObject* hitObject = nullptr;

		/**
		 * \brief Default constructor
		 */
		RaycastHit() = default;

		/**
		 * \brief Constructor
		 * \param ray The ray
		 * \param point The hit point
		 * \param distance The intersection distance
		 * \param hitObject The object that was hit
		 */
		RaycastHit(const Ray ray, const glm::vec3 point, const float distance, GameObject* hitObject)
		{
			this->ray = ray;
			this->point = point;
			this->distance = distance;
			this->hitObject = hitObject;
		}
	};

	/**
	 * \brief 
	 */
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

		/**
		 * \brief Tests for intersections between the ray and game objects in the scene
		 * \param ray The ray
		 * \param out Hit information
		 * \param scene The scene the ray should be cast in
		 * \return Whether the ray hit an object
		 */
		static bool Raycast(Ray ray, RaycastHit& out, Scene* scene);
	};
}