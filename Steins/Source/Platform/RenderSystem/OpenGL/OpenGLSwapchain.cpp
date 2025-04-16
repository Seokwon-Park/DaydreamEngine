#include "SteinsPCH.h"
#include "OpenGLSwapchain.h"

#include "Steins/Core/Window.h"
#include "GLFW/glfw3.h"

#include "glad/glad.h"


namespace Steins
{
	OpenGLSwapchain::OpenGLSwapchain(SwapChainDesc* _desc, SteinsWindow* _window)
	{
		desc = *_desc;
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
}
