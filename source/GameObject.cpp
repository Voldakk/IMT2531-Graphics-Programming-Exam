#include "GameObject.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Scene.hpp"

Component::Component()
{
	
}

void Component::Update(float deltaTime)
{

}

void Component::Render()
{

}


GameObject::GameObject()
{
	transform = std::make_unique<Transform>();
	transform->gameObject = this;
}

void GameObject::Update(float deltaTime)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Render();
	}
}

Transform::Transform()
{
	UpdateModelMatrix();
}

void Transform::Translate(glm::vec3 offset)
{
	position += offset;
	UpdateModelMatrix();
}

void Transform::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
	UpdateModelMatrix();
}

void Transform::Rotate(glm::vec3 offset)
{
	rotation += offset;
	UpdateModelMatrix();
}

void Transform::SetRotation(glm::vec3 newRotation)
{
	rotation = newRotation;
	UpdateModelMatrix();
}

void Transform::Scale(glm::vec3 offset)
{
	scale += offset;
	UpdateModelMatrix();
}

void Transform::SetScale(glm::vec3 newScale)
{
	scale = newScale;
	UpdateModelMatrix();
}

void Transform::UpdateModelMatrix()
{
	model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
}

glm::mat4 Transform::GetModelMatrix()
{
	return model;
}
