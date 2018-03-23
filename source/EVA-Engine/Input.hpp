#pragma once

#include <map>

#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"

// Handles all input in the game
class Input
{
    public:

		/// <summary>Sets the window to capture input from</summary>
		/// <param name="window"></param>  
        static void SetWindow(GLFWwindow* window);

		/// <summary>Clears the recorded states. Should be called every frame immediately before glfwPollEvents()</summary>
		static void Clear();


		// ========== KEYBOARD ==========


		/// <summary>A GLFWkeyfun callback called whenever a key is pressed, repeated or released 
		/// <para>Used to capure input for the KeyDown and KeyUp functions</para> 
		/// </summary>
		/// <param name="window">The window that received the event</param>  
		/// <param name="key">The keyboard key that was pressed or released</param>  
		/// <param name="scancode">The system-specific scancode of the key</param>  
		/// <param name="action">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT</param>  
		/// <param name="mods">Bit field describing which modifier keys were held down</param>
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/// <summary>Checks whether the key is currently held down</summary>
		/// <param name="key">The key</param>  
		/// <returns>Whether the key is currently held down</returns>  
		static bool Key(int key);

		/// <summary>Checks whether the key was pressed this frame</summary>
		/// <param name="key">The key</param>  
		/// <returns>Whether the key was pressed this frame</returns>  
		static bool KeyDown(int key);

		/// <summary>Checks whether the key was released this frame</summary>
		/// <param name="key">The key</param>  
		/// <returns>Whether the key was released this frame</returns>  
		static bool KeyUp(int key);


		// ========== MOUSE POSITION ==========


		static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

		/// <summary>Gets the current position of the cursor</summary>
		/// <returns>The position</returns>  
		static glm::vec2 MousePosition();

		/// <summary>Gets the mouse movement during the last frame</summary>
		/// <returns>The position</returns>  
		static glm::vec2 MouseMovement();


		// ========== MOUSE BUTTONS ==========


		/// <summary>A GLFWmousebuttonfun callback called whenever a mouse button is pressed, repeated or released 
		/// <para>Used to capure input for the MouseButtonDown and MouseButtonUp functions</para> 
		/// </summary>
		/// <param name="window">The window that received the event</param>  
		/// <param name="button">The mouse button that was pressed or released</param>  
		/// <param name="action">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT</param>  
		/// <param name="mods">Bit field describing which modifier keys were held down</param>
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		/// <summary>Checks whether the mouse button is currently held down</summary>
		/// <param name="key">The mouse button</param>  
		/// <returns>Whether the mouse button is currently held down</returns>  
		static bool MouseButton(int button);

		/// <summary>Checks whether the mouse button was pressed this frame</summary>
		/// <param name="key">The mouse button</param>  
		/// <returns>Whether the mouse button was pressed this frame</returns>  
		static bool MouseButtonDown(int button);

		/// <summary>Checks whether the mouse button was released this frame</summary>
		/// <param name="key">The mouse button</param>  
		/// <returns>Whether the mouse button was released this frame</returns>  
		static bool MouseButtonUp(int button);


		// ========== MOUSE SCROLL ==========


		/// <summary>A GLFWmousebuttonfun callback called whenever a mouse button is pressed, repeated or released 
		/// <para>Used to capure input for the GetMouseDown and MouseButtonUp functions</para> 
		/// </summary>
		/// <param name="window">The window that received the event</param>  
		/// <param name="xoffset">The mouse button that was pressed or released</param>  
		/// <param name="yoffset">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT</param>
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		/// <summary>Checks whether the mouse button was released this frame</summary>
		/// <returns>Whether the mouse button was released this frame</returns>  
		static glm::vec2 GetScroll();

    private:
        static GLFWwindow* window;
		static std::map <int, int> keyStates;
		static glm::vec2 scroll;
		static glm::vec2 lastMousePosition;
		static glm::vec2 mouseOffset;
};