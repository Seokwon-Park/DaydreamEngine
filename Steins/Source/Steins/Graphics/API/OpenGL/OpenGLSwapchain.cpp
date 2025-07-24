#include "SteinsPCH.h"
#include "OpenGLSwapchain.h"

#include "Steins/Core/Window.h"
#include "GLFW/glfw3.h"

#include "glad/glad.h"


namespace Steins
{
	OpenGLSwapchain::OpenGLSwapchain(SteinsWindow* _window, const SwapchainDesc& _desc)
	{
		desc = _desc;
		window = (GLFWwindow*)_window->GetNativeWindow();
		glfwMakeContextCurrent(window);
	}
	OpenGLSwapchain::~OpenGLSwapchain()
	{
	}
	void OpenGLSwapchain::SetVSync(bool _enabled)
	{
		glfwSwapInterval(desc.isVSync);
	}

	void OpenGLSwapchain::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}
	void OpenGLSwapchain::ResizeSwapchain(UInt32 _width, UInt32 height)
	{
	}
	void OpenGLSwapchain::BeginFrame()
	{
	}
	void OpenGLSwapchain::EndFrame()
	{
	}
}
