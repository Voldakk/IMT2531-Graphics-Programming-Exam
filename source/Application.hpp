#pragma once

#include <memory>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"

#include "Camera.hpp"
#include "GameObject.hpp"

class Application
{
public:

	static void Init();

	static bool CreateWindow();

	static void Loop();

	/// <summary>Gets the projection matrix for the current window size</summary>
	static glm::mat4 GetPerspectiveMatrix();

	/// <summary>Gets the projection matrix for the current window size</summary>
	static glm::mat4 GetOrthographicMatrix();

	/// <summary>Callback for when the window is resized</summary>
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

	/// <summary>Exits the application</summary>
	static void Exit();

	static std::shared_ptr<Camera> mainCamera;
	static std::shared_ptr<Camera> defaultCamera;

private:
	static GameObject defaultCameraObject;

	static glm::ivec2 windowSize;
	static GLFWwindow * window;

	static glm::mat4 ortoProjection;
	static glm::mat4 perspectiveProjection;
	
};