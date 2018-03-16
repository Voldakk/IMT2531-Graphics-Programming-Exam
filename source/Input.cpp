#include "Input.hpp"

#include "GLFW/glfw3.h"

GLFWwindow* Input::window = nullptr;
std::map <int, int> Input::keyStates;

void Input::SetWindow(GLFWwindow* window)
{
	Input::window = window;
	glfwSetKeyCallback(window, KeyCallback);
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyStates[key] = action;
}

void Input::ClearKeys()
{
	keyStates.clear();
}

bool Input::GetKey(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::GetKeyDown(int key)
{
	if (keyStates.count(key))
		return keyStates[key] == GLFW_PRESS;
	return false;
}

bool Input::GetKeyUp(int key)
{
	if (keyStates.count(key))
		return keyStates[key] == GLFW_RELEASE;
	return false;
}