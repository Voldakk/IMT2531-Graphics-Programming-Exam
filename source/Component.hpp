#pragma once

class GameObject;

class Component
{
public:

	GameObject * gameObject;

	Component();
	virtual void Update(float deltaTime);
	virtual void Render();
};