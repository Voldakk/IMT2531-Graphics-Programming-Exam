#include "Input.hpp"

#include "GLFW/glfw3.h"

GLFWwindow* Input::window = nullptr;
std::map <int, int> Input::keyStates;
glm::vec2 Input::scroll;
glm::vec2 Input::lastMousePosition;
glm::vec2 Input::mouseOffset;

void Input::SetWindow(GLFWwindow* window)
{
	Input::window = window;

	glfwSetKeyCallback(window, KeyCallback);

	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// ========== KEYBOARD ==========

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyStates[key] = action;
}

void Input::Clear()
{
	keyStates.clear();
	scroll = glm::vec2(0.0f, 0.0f);
	mouseOffset = glm::vec2(0.0f, 0.0f);
}

bool Input::Key(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::KeyDown(int key)
{
	if (keyStates.count(key))
		return keyStates[key] == GLFW_PRESS;
	return false;
}

bool Input::KeyUp(int key)
{
	if (keyStates.count(key))
		return keyStates[key] == GLFW_RELEASE;
	return false;
}

// ========== MOUSE POSITION ==========

void Input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec2 newPos = glm::vec2(xpos, ypos);

	if (lastMousePosition == glm::vec2(0.0f))
	{
		lastMousePosition = newPos;
		return;
	}

	mouseOffset = newPos - lastMousePosition;

	lastMousePosition = newPos;
}

glm::vec2 Input::MousePosition()
{
	return lastMousePosition;
}

glm::vec2 Input::MouseMovement()
{
	return mouseOffset;
}

// ========== MOUSE BUTTONS ==========

void Input::MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	keyStates[button] = action;
}

bool Input::MouseButton(int button)
{
	int state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

bool Input::MouseButtonDown(int button)
{
	if (keyStates.count(button))
		return keyStates[button] == GLFW_PRESS;
	return false;
}

bool Input::MouseButtonUp(int button)
{
	if (keyStates.count(button))
		return keyStates[button] == GLFW_RELEASE;
	return false;
}

// ========== MOUSE SCROLL ==========

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll.x = (float)xoffset;
	scroll.y = (float)yoffset;
}

glm::vec2 Input::GetScroll()
{
	return scroll;
}
