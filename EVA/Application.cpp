#include "Application.hpp"

#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

#include "glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "SceneManager.hpp"

namespace EVA
{

    std::shared_ptr<Camera> Application::mainCamera;
    std::shared_ptr<Camera> Application::m_DefaultCamera;

    GameObject Application::m_DefaultCameraObject(nullptr);

    glm::ivec2 Application::m_WindowSize;
    GLFWwindow *Application::m_Window;

    glm::mat4 Application::m_OrthographicProjection;
    glm::mat4 Application::m_PerspectiveProjection;

    void Application::Init(const std::string &title)
    {
        if (!CreateWindow(title))
            return;

        // Camera
        m_DefaultCamera = m_DefaultCameraObject.AddComponent<Camera>();
        mainCamera = m_DefaultCamera;

        glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);

        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);

        WindowResizeCallback(m_Window, width, height);

        // Input
        Input::SetWindow(m_Window);

		// Material stuff
		Material::Init();
    }

    bool Application::CreateWindow(const std::string &title)
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

        m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);

        if (m_Window == nullptr)
        {
            fprintf(stderr, "Failed to open GLFW window.\n");
            getchar();
            glfwTerminate();
            return false;
        }
        glewExperimental = GL_TRUE;

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Failed to initialize GLEW\n");
            getchar();
            glfwTerminate();
            return false;
        }

        glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSwapInterval(0);

        // Set OpenGL options
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // The window's background color
        glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);

        return true;
    }

    void Application::Run()
    {
        auto currentFrameTime = glfwGetTime();
        auto lastFrameTime = currentFrameTime;

        int frameCounter = 0;
        auto lastFpsTime = lastFrameTime;

        while (glfwWindowShouldClose(m_Window) == 0)
        {
            // Measure delta time
            currentFrameTime = glfwGetTime();
            const auto deltaTime = (float) (currentFrameTime - lastFrameTime);
            lastFrameTime = currentFrameTime;

            frameCounter++;
            if (LOG_FPS && currentFrameTime > lastFpsTime + LOG_FPS_INTERVAL)
            {
                std::cout << std::fixed;
                std::cout.precision(1);

                std::cout << "FPS:" << std::setw(5) << frameCounter / LOG_FPS_INTERVAL << "\n";

                lastFpsTime = currentFrameTime;
                frameCounter = 0;
            }


            // ==== UPDATE ====
            SceneManager::Update(deltaTime);

            // ==== RENDER ====
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SceneManager::Render();

            // Swap buffers
            glfwSwapBuffers(m_Window);

            // Input
            Input::Clear();
            glfwPollEvents();

            // Limit fps
            const auto sleepDur = (1.0 / MAX_FPS) - deltaTime;
            if (sleepDur > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds((int) (sleepDur * 1000)));
        }

        // Destroy the window and exit
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::WindowResizeCallback(GLFWwindow *window, int width, int height)
    {
        m_WindowSize = glm::ivec2(width, height);

        glViewport(0, 0, width, height);

        // Projection matrices
        if (m_WindowSize.x > m_WindowSize.y)
            m_OrthographicProjection = glm::ortho(-(float) m_WindowSize.x / (float) m_WindowSize.y,
                                                  (float) m_WindowSize.x / (float) m_WindowSize.y, -1.0f, 1.0f, -1.0f,
                                                  1.0f);
        else
            m_OrthographicProjection = glm::ortho(-1.0f, 1.0f, -(float) m_WindowSize.y / (float) m_WindowSize.x,
                                                  (float) m_WindowSize.y / (float) m_WindowSize.x, -1.0f, 1.0f);

        if (mainCamera != nullptr)
            m_PerspectiveProjection = glm::perspective(glm::radians(mainCamera->fov),
                                                       (float) m_WindowSize.x / (float) m_WindowSize.y,
                                                       mainCamera->near, mainCamera->far);
    }

    void Application::Exit()
    {
        glfwSetWindowShouldClose(m_Window, 1);
    }

}