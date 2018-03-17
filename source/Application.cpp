#include "Application.hpp"

#include <chrono>
#include <thread>
#include <stdio.h>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "Sprite.hpp"
#include "GlobalVars.hpp"
#include "SceneManager.hpp"
#include "MainMenu.hpp"
#include "Game.hpp"
#include "Camera.hpp"

glm::ivec2 Application::windowSize;
GLFWwindow * Application::window;
Camera Application::camera;

void Application::Init()
{
	if (!CreateWindow())
		return;

	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	windowSize = glm::ivec2(width, height);

	// Input
	Input::SetWindow(window);

	// Initialize shared sprite resources
	Sprite::Init();

	// Load starting scene
	SceneManager::CreateScene<StartScene>();

	// Run the window loop
	Loop();
}

bool Application::CreateWindow()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	window = glfwCreateWindow(mode->width, mode->height, APPLICATION_NAME.c_str(), glfwGetPrimaryMonitor(), NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return false;
	}
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1);

	// Set OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// The window's background color
	glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], BACKGROUND_COLOR[3]);

	return true;
}

void Application::Loop()
{
	// Time variables
	float deltaTime;
	double currentFrameTime = glfwGetTime();
	double lastFrameTime = currentFrameTime;

	while (glfwWindowShouldClose(window) == 0)
	{
		// Measure delta time 
		currentFrameTime = glfwGetTime();
		deltaTime = (float)(currentFrameTime - lastFrameTime);
		lastFrameTime = currentFrameTime;

		// ==== UPDATE ====
		SceneManager::Update(deltaTime);

		// ==== RENDER ====
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SceneManager::Render();

		// Swap buffers
		glfwSwapBuffers(window);

		// Input
		Input::Clear();
		glfwPollEvents();

		// Limit fps
		double sleepDur = (1.0 / MAX_FPS) - deltaTime;
		if (sleepDur > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(sleepDur * 1000)));
	}

	// Destroy the window and exit
	glfwDestroyWindow(window);
	glfwTerminate();
}

glm::mat4 Application::GetProjectionMatrix(bool orto)
{
	if (orto)
	{
		if (windowSize.x > windowSize.y)
			return glm::ortho(-(float)windowSize.x / (float)windowSize.y, (float)windowSize.x / (float)windowSize.y, -1.0f, 1.0f, -1.0f, 1.0f);
		else
			return glm::ortho(-1.0f, 1.0f, -(float)windowSize.y / (float)windowSize.x, (float)windowSize.y / (float)windowSize.x, -1.0f, 1.0f);
	}
	return  glm::perspective(camera.fov, (float)windowSize.x / (float)windowSize.y, 0.1f, 100.0f);
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
