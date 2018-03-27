#pragma once

#include "glm/glm/glm.hpp"

#include "Component.hpp"
#include <vector>
#include <memory>

namespace EVA
{

	class GameObject;

	class Transform : public Component
	{

	private:

		glm::mat4 m_Model;

		std::shared_ptr<Transform> m_Parent;
		std::vector<Transform *> m_Children;

	public:

		glm::vec3 localPosition = glm::vec3(0.0f);
		glm::vec3 localRotation = glm::vec3(0.0f);
		glm::vec3 localScale = glm::vec3(1.0f);

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		explicit Transform(GameObject *gameObject);

		void Translate(glm::vec3 offset);

		void SetPosition(glm::vec3 newPosition);

		void Rotate(glm::vec3 offset);

		void SetRotation(glm::vec3 newRotation);

		void Scale(glm::vec3 offset);

		void SetScale(glm::vec3 newScale);

		void UpdateModelMatrix();

		inline glm::mat4 GetModelMatrix() const
		{
			return m_Model;
		}

		void SetParent(GameObject *newParent);

		void SetParent(std::shared_ptr<Transform> newParent);

		unsigned int GetChildIndex(Transform *child) const;

		unsigned int GetChildIndex(GameObject *child) const;
	};

}