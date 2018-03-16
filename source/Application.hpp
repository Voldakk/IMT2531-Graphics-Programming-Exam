#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"

class Application
{
public:

	/// <summary>Gets the projection matrix for the current window size</summary>
	static glm::mat4 GetProjectionMatrix();

	/// <summary>Sets the window used by the application</summary>
	static void SetWindow(GLFWwindow * newWindow);

	/// <summary>Callback for when the window is resized</summary>
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

	/// <summary>Exits the application</summary>
	static void Exit();

private:
	static glm::ivec2 windowSize;
	static GLFWwindow * window;
};