#include "Physics.hpp"

#include "Scene.hpp"	

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace EVA
{

	Ray Physics::ScreenPosToWorldRay(glm::ivec2 mousePos, const EVA::Camera* camera)
	{
		const auto windowSize = Application::GetWindowSize();
		const auto projectionMatrix = Application::GetPerspectiveMatrix();
		const auto viewMatrix = camera->viewMatrix;

		mousePos.y = windowSize.y - mousePos.y;

		// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
		const glm::vec4 lRayStartNdc
		(
			((float)mousePos.x / (float)windowSize.x - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
			((float)mousePos.y / (float)windowSize.y - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
			-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
			1.0f
		);

		const glm::vec4 lRayEndNdc
		(
			((float)mousePos.x / (float)windowSize.x - 0.5f) * 2.0f,
			((float)mousePos.y / (float)windowSize.y - 0.5f) * 2.0f,
			0.0,
			1.0f
		);

		// The Projection matrix goes from Camera Space to NDC.
		// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
		const auto inverseProjectionMatrix = glm::inverse(projectionMatrix);

		// The View Matrix goes from World Space to Camera Space.
		// So inverse(ViewMatrix) goes from Camera Space to World Space.
		const auto inverseViewMatrix = glm::inverse(viewMatrix);

		// Faster way (just one inverse)
		const auto m = glm::inverse(projectionMatrix * viewMatrix);

		auto lRayStartWorld = m * lRayStartNdc;
		lRayStartWorld /= lRayStartWorld.w;

		auto lRayEndWorld = m * lRayEndNdc;
		lRayEndWorld /= lRayEndWorld.w;

		glm::vec3 lRayDirWorld(lRayEndWorld - lRayStartWorld);
		lRayDirWorld = glm::normalize(lRayDirWorld);

		// Return the ray
		return { glm::vec3(lRayStartWorld), glm::normalize(lRayDirWorld) };
	}

	bool Physics::TestRayObbIntersection(const Ray ray, const glm::vec3 aabbMin, const glm::vec3 aabbMax, const glm::mat4 modelMatrix, float& intersectionDistance)
	{
		// Intersection method from Real-Time Rendering and Essential Mathematics for Games
		auto tMin = 0.0f;
		auto tMax = 100000.0f;

		const glm::vec3 obbPositionWorldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

		const auto delta = obbPositionWorldspace - ray.origin;

		// Test intersection with the 2 planes perpendicular to the OBB's X axis
		{
			const glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
			const auto e = glm::dot(xaxis, delta);
			const auto f = glm::dot(ray.direction, xaxis);

			if (fabs(f) > 0.001f) // Standard case
			{
				// Intersection with the "left" plan
				auto t1 = (e + aabbMin.x) / f;

				// Intersection with the "right" plan
				auto t2 = (e + aabbMax.x) / f;

				// t1 and t2 now contain distances betwen ray origin and ray-plane intersections
				// We want t1 to represent the nearest intersection, 
				// so if it's not the case, invert t1 and t2

				if (t1 > t2)
				{
					const auto w = t1;
					t1 = t2;
					t2 = w; // swap t1 and t2
				}

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				if (t2 < tMax)
					tMax = t2;
				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				if (t1 > tMin)
					tMin = t1;

				// And here's the trick:
				// If "far" is closer than "near", then there is NO intersection.
				// See the images in the tutorials for the visual explanation.
				if (tMax < tMin)
					return false;
			}
			else // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			{
				if (-e + aabbMin.x > 0.0f || -e + aabbMax.x < 0.0f)
					return false;
			}
		}

		// Test intersection with the 2 planes perpendicular to the OBB's Y axis
		// Exactly the same thing than above.
		{
			const glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
			const auto e = glm::dot(yaxis, delta);
			const auto f = glm::dot(ray.direction, yaxis);

			if (fabs(f) > 0.001f)
			{
				auto t1 = (e + aabbMin.y) / f;
				auto t2 = (e + aabbMax.y) / f;

				if (t1 > t2)
				{
					const auto w = t1;
					t1 = t2;
					t2 = w;
				}

				if (t2 < tMax)
					tMax = t2;
				if (t1 > tMin)
					tMin = t1;
				if (tMin > tMax)
					return false;
			}
			else
			{
				if (-e + aabbMin.y > 0.0f || -e + aabbMax.y < 0.0f)
					return false;
			}
		}

		// Test intersection with the 2 planes perpendicular to the OBB's Z axis
		// Exactly the same thing than above.
		{
			const glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
			const auto e = glm::dot(zaxis, delta);
			const auto f = glm::dot(ray.direction, zaxis);

			if (fabs(f) > 0.001f)
			{
				auto t1 = (e + aabbMin.z) / f;
				auto t2 = (e + aabbMax.z) / f;

				if (t1 > t2)
				{
					const auto w = t1;
					t1 = t2;
					t2 = w;
				}

				if (t2 < tMax)
					tMax = t2;
				if (t1 > tMin)
					tMin = t1;
				if (tMin > tMax)
					return false;
			}
			else
			{
				if (-e + aabbMin.z > 0.0f || -e + aabbMax.z < 0.0f)
					return false;
			}
		}

		intersectionDistance = tMin;
		return true;
	}

	bool Physics::Raycast(const Ray ray, RaycastHit& out, Scene* scene)
	{
		auto minDist = 999999.0f;
		EVA::GameObject* go = nullptr;

		for (const auto& gameObject : scene->GetGameObjects())
		{
			if (gameObject->GetName() == "Camera")
				continue;;

			float intersectionDistance;
			const auto aabbMin = -gameObject->transform->scale;
			const auto aabbMax = gameObject->transform->scale;

			const auto rotationMatrix = glm::toMat4(gameObject->transform->orientation);
			const auto translationMatrix = glm::translate(glm::mat4(), gameObject->transform->position);
			const auto modelMatrix = translationMatrix * rotationMatrix;

			if (TestRayObbIntersection(ray, aabbMin, aabbMax, modelMatrix, intersectionDistance))
			{
				if (intersectionDistance < minDist)
				{
					minDist = intersectionDistance;
					go = gameObject.get();
				}
			}
		}

		if (go != nullptr)
		{
			const auto point = ray.origin + ray.direction * minDist;

			out = RaycastHit(ray, point, minDist, go);
			return true;
		}

		return false;
	}
}