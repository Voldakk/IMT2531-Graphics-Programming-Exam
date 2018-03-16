#pragma once
#include <map>

#include "GLFW/glfw3.h"

// Handles all input in the game
class Input
{
    public:

		/// <summary>Sets the window to capture input from</summary>
		/// <param name="window"></param>  
        static void SetWindow(GLFWwindow* window);

		/// <summary>A GLFWkeyfun callback called whenever a key is pressed, repeated or released 
		/// <para>Used to capure input for the GetKeyDown and GetKeyUp functions</para> 
		/// </summary>
		/// <param name="window">The window that received the event</param>  
		/// <param name="key">The keyboard key that was pressed or released</param>  
		/// <param name="scancode">The system-specific scancode of the key</param>  
		/// <param name="action">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT</param>  
		/// <param name="mods">Bit field describing which modifier keys were held down</param>
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/// <summary>Clears the recorded key states. Should be called every frame immediately before glfwPollEvents()</summary>
		static void ClearKeys();

		/// <summary>Checks whether the key is currently held down</summary>
		/// <param name="key">The key</param>  
		/// <returns>Whether the key is currently held down</returns>  
		static bool GetKey(int key);

		/// <summary>Checks whether the key was pressed this frame</summary>
		/// <param name="key">The key</param>  
		/// <returns>Whether the key was pressed this frame</returns>  
		static bool GetKeyDown(int key);

		/// <summary>Checks whether the key was released this frame</summary>
		/// <param name="key">The key</param>  
		/// <returns>Whether the key was released this frame</returns>  
		static bool GetKeyUp(int key);


    private:
        static GLFWwindow* window;
		static std::map <int, int> keyStates;
};