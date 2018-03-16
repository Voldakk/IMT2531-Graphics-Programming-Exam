#include <chrono>
#include <thread>
#include <stdio.h>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Input.hpp"
#include "Sprite.hpp"
#include "MainMenu.hpp"
#include "GlobalVars.hpp"
#include "Application.hpp"
#include "SceneManager.hpp"

typedef MainMenu StartScene;

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSwapInterval(1);

	// Set OpenGL options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// The window's background color
	glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], 1);

	// Time variables
	float deltaTime;
	double currentFrameTime = glfwGetTime();
	double lastFrameTime = currentFrameTime;

	// Application
	Application::SetWindow(window);

	// Input
	Input::SetWindow(window);

	// Initialize shared sprite resources
	Sprite::Init();

	// Load starting scene
	SceneManager::LoadScene(std::make_shared<StartScene>());

	while(glfwWindowShouldClose(window) == 0)
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
		Input::ClearKeys();
		glfwPollEvents();

		// Limit fps
		double sleepDur = (1.0 / MAX_FPS)-deltaTime;
		if(sleepDur > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(sleepDur * 1000)));
	}

	// Destroy the window and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}