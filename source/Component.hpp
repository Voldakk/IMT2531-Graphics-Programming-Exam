#pragma once

class GameObject;

class Component
{
public:
	GameObject * gameObject;

	Component(GameObject * gameObject);
	virtual void Update(float deltaTime);
	virtual void Render();
};