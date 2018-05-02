#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "../Component.hpp"

namespace EVA
{
	const glm::vec3 XAXIS = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 YAXIS = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 ZAXIS = { 0.0f, 0.0f, 1.0f };

	class GameObject;

	class Transform : public Component
	{
		glm::mat4 m_ModelMatrix;

		Transform* m_Parent;
		std::vector<Transform *> m_Children;

		glm::vec3 m_LocalPosition = glm::vec3(0.0f);
		glm::quat m_LocalOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 m_LocalScale = glm::vec3(1.0f);

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::quat m_Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 m_Scale = glm::vec3(1.0f);

		glm::vec3 m_Forward;
		glm::vec3 m_Right;
		glm::vec3 m_Up;


	public:

		// Public read only fields
		const glm::mat4& modelMatrix = m_ModelMatrix;

		const glm::vec3& localPosition = m_LocalPosition;
		const glm::quat& localOrientation = m_LocalOrientation;
		const glm::vec3& localScale = m_LocalScale;

		const glm::vec3& position = m_Position;
		const glm::quat& orientation = m_Orientation;
		const glm::vec3& scale = m_Scale;

		const glm::vec3& forward = m_Forward;
		const glm::vec3& right = m_Right;
		const glm::vec3& up = m_Up;

		/**
		 * \brief Constructor
		 */
		Transform();

		/**
		* \brief Translates the transform by the given offset
		* \param offset The amount the transform should be translated
		*/
		void Translate(glm::vec3 offset);

		/**
		* \brief Translates the transform by the given offset
		* \param offset The amount the transform should be translated
		*/
		void Translate(glm::vec2 offset);

		/**
		* \brief Sets the transfrom's position
		* \param newPosition The new position
		*/
		void SetPosition(glm::vec3 newPosition);


		/**
		* \brief Sets the transfrom's position
		* \param newPosition The new position
		*/
		void SetPosition(glm::vec2 newPosition);

		/**
		 * \brief Rotates the transform by the given offset
		 * \param offset 
		 */
		void Rotate(glm::quat offset);

		/**
		* \brief Rotates the transform around an axis
		* \param axis The axis to rotate around
		* \param angle The angle in degrees
		*/
		void Rotate(glm::vec3 axis, float angle);

		/**
		* \brief Rotates the transform around the Z axis
		* \param angle The angle in degrees
		*/
		void Rotate(float angle);

		/**
		 * \brief Sets the transform's orientation
		 * \param newOrientation The new orientation
		 */
		void SetOrientation(glm::quat newOrientation);

		/**
		* \brief Sets the transform's orientation defined by a rotation around an axis
		* \param axis The axis to rotate around
		* \param angle The angle in degrees
		*/
		void SetOrientation(glm::vec3 axis, float angle);

		/**
		* \brief Sets the transform's orientation defined by a rotation around the Z axis
		* \param angle The angle in degrees
		*/
		void SetOrientation(float angle);

		/**
		 * \brief Scales the transform by a given amount on each axis
		 * \param amount The amount
		 */
		void Scale(glm::vec3 amount);

		/**
		 * \brief Scales the transform by a given amount
		 * \param amount The amount
		 */
		void Scale(float amount);

		/**
		 * \brief Sets the transform's scale
		 * \param newScale The new scale
		 */
		void SetScale(glm::vec3 newScale);

		/**
		 * \brief Sets the transform's scale
		 * \param newScale The new scale
		 */
		void SetScale(float newScale);

		/**
		 * \brief Recalculates the transform's model matrix (and all children recursively)
		 */
		void UpdateModelMatrix();

		/**
		 * \brief Sets the transform's parent
		 * \param newParent The new parent
		 */
		void SetParent(GameObject *newParent);

		/**
		 * \brief Sets the transform's parent
		 * \param newParent The new parent
		 */
		void SetParent(Transform* newParent);

		/**
		 * \brief Gets the child's index in the transforms list of children
		 * \param child The child
		 * \return The child's index
		 */
		int GetChildIndex(Transform *child) const;

		/**
		* \brief Gets the child's index in the transforms list of children
		* \param child The child
		* \return The child's index
		*/
		int GetChildIndex(GameObject *child) const;

		glm::vec3 LocalToWorld(glm::vec3 localPosition) const;
	};
}