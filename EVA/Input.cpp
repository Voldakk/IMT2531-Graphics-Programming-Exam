#include "Input.hpp"

#include "Application.hpp"
#include "Editor/EditorWindows.hpp"

namespace EVA
{

    GLFWwindow *Input::m_Window = nullptr;
	std::map<int, int> Input::m_KeyStates;
	std::map<int, int> Input::m_ButtonStates;
    glm::vec2 Input::m_Scroll;
    glm::vec2 Input::m_LastMousePosition;
    glm::vec2 Input::m_MouseOffset;

	bool Input::m_ImGui;

    void Input::SetWindow(GLFWwindow *window, const bool imgui)
    {
        m_Window = window;

		m_ImGui = imgui;
		if(!imgui)
		{
	        glfwSetKeyCallback(window, KeyCallback);
	        glfwSetMouseButtonCallback(window, MouseButtonCallback);
		}

	    glfwSetScrollCallback(window, ScrollCallback);
	    glfwSetCursorPosCallback(window, CursorPositionCallback);
    }

	// ========== KEYBOARD ==========

    void Input::KeyCallback(GLFWwindow *window, const int key, const int scancode, const int action, const int mods)
    {
        m_KeyStates[key] = action;
    }

    void Input::Clear()
    {
        m_KeyStates.clear();
		m_ButtonStates.clear();
        m_Scroll = glm::vec2(0.0f, 0.0f);
        m_MouseOffset = glm::vec2(0.0f, 0.0f);
    }

    bool Input::Key(const int key)
    {
        return glfwGetKey(m_Window, key) == GLFW_PRESS;
    }

    bool Input::KeyDown(const int key)
    {
		if (m_ImGui)
			return ImGui::IsKeyPressed(key);

        if (m_KeyStates.count(key))
            return m_KeyStates[key] == GLFW_PRESS;
        return false;
    }

    bool Input::KeyUp(const int key)
    {
		if (m_ImGui)
			return ImGui::IsKeyReleased(key);

        if (m_KeyStates.count(key))
            return m_KeyStates[key] == GLFW_RELEASE;
        return false;
    }

	void Input::SetCursorMode(const CursorMode mode)
	{
		switch (mode)
		{
		case Normal: 
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			return;
		case Hidden:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			return;
		case Disabled:
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			return;
		}
		
	}

	// ========== MOUSE POSITION ==========

    void Input::CursorPositionCallback(GLFWwindow *window, const double xpos, const double ypos)
    {
        const auto newPos = glm::vec2(xpos, ypos);

        if (m_LastMousePosition == glm::vec2(0.0f))
        {
            m_LastMousePosition = newPos;
            return;
        }

        m_MouseOffset = newPos - m_LastMousePosition;

        m_LastMousePosition = newPos;
    }

	glm::vec2 Input::ScreenToWorldPos(glm::vec2 pos)
	{
		const auto screenSize = (glm::vec2)Application::GetWindowSize();

		pos -= screenSize / 2.0f;	// Move (0, 0) to the center of the screen
		pos.y *= -1.0f;				// Flipp y-axis
		pos /= screenSize.y / 2.0f;	// Normalize to -1 ... +1 on the y-axis, and whatever the aspect ratio is on the x-axis

		return pos;
	}

	// ========== MOUSE BUTTONS ==========

    void Input::MouseButtonCallback(GLFWwindow *window, const int button, const int action, const int mods)
    {
		m_ButtonStates[button] = action;
    }

    bool Input::MouseButton(const int button)
    {
        const auto state = glfwGetMouseButton(m_Window, button);
        return state == GLFW_PRESS;
    }

    bool Input::MouseButtonDown(const int button)
    {
		if (m_ImGui)
			return ImGui::IsMouseClicked(button);

        if (m_ButtonStates.count(button))
            return m_ButtonStates[button] == GLFW_PRESS;
        return false;
    }

    bool Input::MouseButtonUp(const int button)
    {
		if (m_ImGui)
			return ImGui::IsMouseReleased(button);

        if (m_ButtonStates.count(button))
            return m_ButtonStates[button] == GLFW_RELEASE;
        return false;
    }

	// ========== MOUSE SCROLL ==========

    void Input::ScrollCallback(GLFWwindow *window, const double xoffset, const double yoffset)
    {
        m_Scroll.x = (float) xoffset;
        m_Scroll.y = (float) yoffset;
    }

}
