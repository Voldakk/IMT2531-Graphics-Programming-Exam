#pragma once

#include <memory>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "Camera.hpp"
#include "GameObject.hpp"

namespace EVA
{

    const int MAX_FPS = 2000;
    const int WINDOW_WIDTH = 1664;
    const int WINDOW_HEIGHT = 936;
    const glm::vec4 CLEAR_COLOR = glm::vec4{0.5f, 0.5f, 0.5f, 1.0f};

    const bool LOG_FPS = true;
    const double LOG_FPS_INTERVAL = 1.0;

    class Application
    {

        static Camera* m_DefaultCamera;

        static GameObject m_DefaultCameraObject;

        static glm::ivec2 m_WindowSize;
        static GLFWwindow *m_Window;

        static glm::mat4 m_OrthographicProjection;
        static glm::mat4 m_PerspectiveProjection;

    public:

        static Camera* mainCamera;

        /// <summary>Initializes the application</summary>
        static void Init(const std::string &title);

        /// <summary>Starts the game loop</summary>
        static void Run();

        /// <summary>Exits the application</summary>
        static void Exit();

        /// <summary>Gets the perspective projection matrix for the current window size and camera fov</summary>
        static inline glm::mat4 GetPerspectiveMatrix()
        { return m_PerspectiveProjection; }

        /// <summary>Gets the orthographic projection matrix for the current window size</summary>
        static inline glm::mat4 GetOrthographicMatrix()
        { return m_OrthographicProjection; }

		static inline glm::ivec2 GetWindowSize()
		{ return m_WindowSize; }

    private:

        /// <summary>Creates a window and OpenGL context</summary>
        static bool CreateWindow(const std::string &title);

        /// <summary>Callback for when the window is resized</summary>
        static void WindowResizeCallback(GLFWwindow *window, int width, int height);
    };

}