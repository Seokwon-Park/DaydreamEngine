#include "SteinsPCH.h"
#include "WindowsWindow.h"

#include "Steins/Event/KeyEvent.h"
#include "Steins/Event/MouseEvent.h"
#include "Steins/Event/ApplicationEvent.h"


namespace Steins
{
	namespace
	{
		static bool isGLFWInitialized = false;

		static void GLFWErrorCallback(int error, const char* description)
		{
			STEINS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		}
	}

	WindowsWindow::WindowsWindow(const WindowProps& _props)
	{
		Init(_props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& _props)
	{
		windowData.title = _props.title;
		windowData.width = _props.width;
		windowData.height = _props.height;
		windowData.keyStates.resize(GLFW_KEY_LAST + 1, 0);
		windowData.keyDownChecker.resize(GLFW_KEY_LAST + 1, 0);

		std::wstring title(windowData.title.begin(), windowData.title.end());

		STEINS_CORE_INFO("Create Window {0} ({1}, {2})", _props.title, _props.width, _props.height);

		if (false == isGLFWInitialized)
		{
			int success = glfwInit();
			STEINS_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			isGLFWInitialized = true;
		}

		glfwWindow = glfwCreateWindow((int)_props.width, (int)_props.height, _props.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(glfwWindow);
		glfwSetWindowUserPointer(glfwWindow, &windowData);
		SetVSync(true);

		windowHandle = glfwGetWin32Window(glfwWindow);

		glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* _window, int _width, int _height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				data.width = _width;
				data.height = _height;
				WindowResizeEvent event(_width, _height);
				data.eventCallbackFn(event);
			});
		glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* _window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				WindowCloseEvent event;
				data.eventCallbackFn(event);
			});
		glfwSetKeyCallback(glfwWindow, [](GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				switch (_action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(_key, 0);
					data.eventCallbackFn(event);
					data.keyStates[_key] = STEINS_PRESS;
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(_key);
					data.eventCallbackFn(event);
					data.keyStates[_key] = STEINS_RELEASE;
					data.keyDownChecker[_key] = false;
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(_key, 1);
					data.eventCallbackFn(event);
					data.keyStates[_key] = STEINS_REPEAT;
					break;
				}
				}
			});
		glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* _window, int _button, int _action, int _mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);

				switch (_action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(_button);
					data.eventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(_button);
					data.eventCallbackFn(event);
					break;
				}
				}
			});
		glfwSetScrollCallback(glfwWindow, [](GLFWwindow* _window, double _xOffset, double _yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				MouseScrolledEvent event((float)_xOffset, (float)_yOffset);
				data.eventCallbackFn(event);
			});
		glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* _window, double _xPos, double _yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_window);
				MouseMovedEvent event((float)_xPos, (float)_yPos);
				data.eventCallbackFn(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(glfwWindow);
	}
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(glfwWindow);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		windowData.isVSync = enabled;
	}
	bool WindowsWindow::IsVSync() const
	{
		return windowData.isVSync;
	}
	void WindowsWindow::OnUpdateKeyState()
	{
		for (int i = 0; i < GLFW_KEY_LAST; i++)
		{
			if (windowData.keyStates[i] == STEINS_RELEASE)
			{
				windowData.keyStates[i] = STEINS_IDLE;
			}
		}
	}
}
