#pragma once

#include <vector>
#include <memory>

#include "glm/glm/glm.hpp"

class Scene;
class GameObject;

class Component
{
public:

	GameObject * gameObject;

	Component();
	virtual void Update(float deltaTime);
	virtual void Render();
};



class Transform : public Component
{
public:

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Transform();

	void Translate(glm::vec3 offset);
	void SetPosition(glm::vec3 newPosition);

	void Rotate(glm::vec3 offset);
	void SetRotation(glm::vec3 newRotation);

	void Scale(glm::vec3 offset);
	void SetScale(glm::vec3 newScale);

	void UpdateModelMatrix();

	glm::mat4 GetModelMatrix();

private:
	glm::mat4 model;
};



class GameObject
{
public:

	Scene * scene;
	std::unique_ptr<Transform> transform;
	std::vector<std::shared_ptr<Component>> components;

	GameObject();

	void Update(float deltaTime);
	void Render();

	template <class T>
	std::shared_ptr<T> AddComponent();
};

template<class T>
inline std::shared_ptr<T> GameObject::AddComponent()
{
	std::shared_ptr<T> component = std::make_shared<T>();

	component->gameObject = this;
	components.push_back(component);

	return component;
}