#include "Application.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

glm::ivec2 Application::windowSize;
GLFWwindow * Application::window;

glm::mat4 Application::GetProjectionMatrix()
{
	if(windowSize.x > windowSize.y)
		return glm::ortho(-(float)windowSize.x / (float)windowSize.y, (float)windowSize.x / (float)windowSize.y, -1.0f, 1.0f, -1.0f, 1.0f);
	else 
		return glm::ortho(-1.0f, 1.0f, -(float)windowSize.y / (float)windowSize.x, (float)windowSize.y / (float)windowSize.x, -1.0f, 1.0f);
}

void Application::SetWindow(GLFWwindow * newWindow)
{
	window = newWindow;
	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	windowSize = glm::ivec2(width, height);
}

void Application::WindowResizeCallback(GLFWwindow * window, int width, int height)
{
	windowSize = glm::ivec2(width, height);

	glViewport(0, 0, width, height);
}

void Application::Exit()
{
	glfwSetWindowShouldClose(window, 1);
}
