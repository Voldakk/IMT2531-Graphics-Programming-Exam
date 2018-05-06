#pragma once

#include <map>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace EVA
{

	/**
	 * \brief Handles all input
	 */
	class Input
	{

		static GLFWwindow *m_Window;
		static std::map<int, int> m_KeyStates;
		static std::map<int, int> m_ButtonStates;
		static glm::vec2 m_Scroll;
		static glm::vec2 m_LastMousePosition;
		static glm::vec2 m_MouseOffset;

		static bool m_ImGui;

	public:

		/// <summary>Sets the window to capture input from</summary>
		/// <param name="window"></param>
		static void SetWindow(GLFWwindow *window, bool imgui = false);

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
		static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

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


		// ==========   CURSOR MODE   ==========

		enum CursorMode { Normal, Hidden, Disabled };

		static void SetCursorMode(CursorMode mode);


		// ========== MOUSE POSITION ==========


		/**
		 * \brief Callback for mouse position change in a GLFW window
		 * \param window The window
		 * \param xpos Mouse x position
		 * \param ypos Mouse y position
		 */
		static void CursorPositionCallback(GLFWwindow *window, double xpos, double ypos);

		/// <summary>Gets the current position of the cursor from the top left corner of the window</summary>
		/// <returns>The position</returns>
		static inline glm::vec2 MousePosition()
		{ return m_LastMousePosition; }

		/// <summary>Gets the mouse movement during the last frame</summary>
		/// <returns>The position</returns>
		static inline glm::vec2 MouseMovement()
		{ return m_MouseOffset; }

		static glm::vec2 ScreenToWorldPos(glm::vec2 pos);


		// ========== MOUSE BUTTONS ==========


		/// <summary>A GLFWmousebuttonfun callback called whenever a mouse button is pressed, repeated or released
		/// <para>Used to capure input for the MouseButtonDown and MouseButtonUp functions</para>
		/// </summary>
		/// <param name="window">The window that received the event</param>
		/// <param name="button">The mouse button that was pressed or released</param>
		/// <param name="action">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT</param>
		/// <param name="mods">Bit field describing which modifier keys were held down</param>
		static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

		/// <summary>Checks whether the mouse button is currently held down</summary>
		/// <param name="button">The mouse button</param>
		/// <returns>Whether the mouse button is currently held down</returns>
		static bool MouseButton(int button);

		/// <summary>Checks whether the mouse button was pressed this frame</summary>
		/// <param name="button">The mouse button</param>
		/// <returns>Whether the mouse button was pressed this frame</returns>
		static bool MouseButtonDown(int button);

		/// <summary>Checks whether the mouse button was released this frame</summary>
		/// <param name="button">The mouse button</param>
		/// <returns>Whether the mouse button was released this frame</returns>
		static bool MouseButtonUp(int button);


		// ========== MOUSE SCROLL ==========


		/// <summary>A GLFWmousebuttonfun callback called whenever a mouse button is pressed, repeated or released
		/// <para>Used to capure input for the GetMouseDown and MouseButtonUp functions</para>
		/// </summary>
		/// <param name="window">The window that received the event</param>
		/// <param name="xoffset">The mouse button that was pressed or released</param>
		/// <param name="yoffset">GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT</param>
		static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

		/// <summary>Checks whether the mouse button was released this frame</summary>
		/// <returns>Whether the mouse button was released this frame</returns>
		static inline glm::vec2 GetScroll()
		{ return m_Scroll; }
		

		// ==========     KEYS     ==========


		enum Key
		{
			/* Printable keys */

			Space = GLFW_KEY_SPACE,

			Apostrophe = GLFW_KEY_APOSTROPHE,

			Comma = GLFW_KEY_COMMA,
			Minus, Period, Slash,

			Alpha0 = GLFW_KEY_0,
			Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,

			Semicolon = GLFW_KEY_SEMICOLON,

			Equal = GLFW_KEY_EQUAL,

			A = GLFW_KEY_A,
			B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

			LeftBracket = GLFW_KEY_LEFT_BRACKET,
			BackSlash, RightBracket,

			GraveAccent = GLFW_KEY_GRAVE_ACCENT,

			World1 = GLFW_KEY_WORLD_1,
			World2,

			/* Function keys */

			Escape = GLFW_KEY_ESCAPE,
			Enter, Tab, Backspace, Insert, Delete,
			Right, Left, Down, Up,
			PageUp, PageDown, Home, End,

			CapsLock = GLFW_KEY_CAPS_LOCK,
			ScrollLock, NumLock,
			PrintScreen, Pause,

			F1 = GLFW_KEY_F1,
			F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

			Kp0 = GLFW_KEY_KP_0,
			Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,

			KpDecimal = GLFW_KEY_KP_DECIMAL,
			KpDevide, KpMultiply, KpSubtract, KpAdd, KpEnter, KpEqual,

			LeftShift = GLFW_KEY_LEFT_SHIFT,
			LeftControl, LeftAlt, LeftSuper,
			RightShift, RightControl, RightAlt, RightSuper,
			Menu,

			LastKey = Menu
		};

		enum Modifier
		{
			Shift = GLFW_MOD_SHIFT,
			Control = GLFW_MOD_CONTROL,
			Alt = GLFW_MOD_ALT,
			Super = GLFW_MOD_SUPER
		};

		enum Button
		{
			Mouse0 = GLFW_MOUSE_BUTTON_1, 
			Mouse1, Mouse2, Mouse3, Mouse4, Mouse5, Mouse6, Mouse7, Mouse8,

			LastButtton = Mouse8,

			MouseLeft = Mouse0,
			MouseRight = Mouse1,
			MouseMiddle = Mouse3

		};
	};

}