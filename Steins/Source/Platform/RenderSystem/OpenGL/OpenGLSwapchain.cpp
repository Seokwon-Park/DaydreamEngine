#include "SteinsPCH.h"
#include "OpenGLSwapchain.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	OpenGLSwapchain::OpenGLSwapchain(SwapchainDesc* _desc, SteinsWindow* _window)
	{
	}
	OpenGLSwapchain::~OpenGLSwapchain()
	{
	}
	void OpenGLSwapchain::SetVSync(bool _enabled)
	{
	}

	void OpenGLSwapchain::SwapBuffers()
	{
		glfwSwapInterval(desc.isVSync);
	}
}
