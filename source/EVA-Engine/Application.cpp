#include "Application.hpp"

#include <chrono>
#include <thread>
#include <cstdio>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "Sprite.hpp"
#include "GlobalVars.hpp"
#include "SceneManager.hpp"
#include "Camera.hpp"
#include "GameObject.hpp"

std::shared_ptr<Camera> Application::mainCamera;
std::shared_ptr<Camera> Application::defaultCamera;

GameObject Application::defaultCameraObject;

glm::ivec2 Application::windowSize;
GLFWwindow * Application::window;

glm::mat4 Application::ortoProjection;
glm::mat4 Application::perspectiveProjection;

void Application::Init(const std::string& title)
{
	if (!CreateWindow(title))
		return;

	// Camera
	defaultCamera = defaultCameraObject.AddComponent<Camera>();
	mainCamera = defaultCamera;

	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	WindowResizeCallback(window, width, height);	

	// Input
	Input::SetWindow(window);

	// Initialize shared sprite resources
	Sprite::Init();
}

bool Application::CreateWindow(const std::string& title)
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
	//auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//window = glfwCreateWindow(mode->width, mode->height, APPLICATION_NAME.c_str(), glfwGetPrimaryMonitor(), NULL);
	
	window = glfwCreateWindow(1920, 1080, title.c_str(), nullptr, nullptr);
	
	glfwMakeContextCurrent(window);
	
	if (window == nullptr) {
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
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// The window's background color
	glClearColor(BACKGROUND_COLOR[0], BACKGROUND_COLOR[1], BACKGROUND_COLOR[2], BACKGROUND_COLOR[3]);

	return true;
}

void Application::Run()
{
	auto currentFrameTime = glfwGetTime();
	auto lastFrameTime = currentFrameTime;

	while (glfwWindowShouldClose(window) == 0)
	{
		// Measure delta time 
		currentFrameTime = glfwGetTime();
		const auto deltaTime = (float)(currentFrameTime - lastFrameTime);
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
		const auto sleepDur = (1.0 / MAX_FPS) - deltaTime;
		if (sleepDur > 0)
			std::this_thread::sleep_for(std::chrono::milliseconds((int)(sleepDur * 1000)));
	}

	// Destroy the window and exit
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::WindowResizeCallback(GLFWwindow * window, int width, int height)
{
	windowSize = glm::ivec2(width, height);

	glViewport(0, 0, width, height);

	// Projection matrices
	if (windowSize.x > windowSize.y)
		ortoProjection = glm::ortho(-(float)windowSize.x / (float)windowSize.y, (float)windowSize.x / (float)windowSize.y, -1.0f, 1.0f, -1.0f, 1.0f);
	else
		ortoProjection = glm::ortho(-1.0f, 1.0f, -(float)windowSize.y / (float)windowSize.x, (float)windowSize.y / (float)windowSize.x, -1.0f, 1.0f);
	
	if(mainCamera != nullptr)
		perspectiveProjection = glm::perspective(mainCamera->fov, (float)windowSize.x / (float)windowSize.y, 0.1f, 100.0f);
}

void Application::Exit()
{
	glfwSetWindowShouldClose(window, 1);
}
