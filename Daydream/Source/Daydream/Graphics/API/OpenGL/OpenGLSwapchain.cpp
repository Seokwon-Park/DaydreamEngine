#include "DaydreamPCH.h"
#include "OpenGLSwapchain.h"

#include "Daydream/Core/Window.h"
#include "GLFW/glfw3.h"

#include "glad/glad.h"


namespace Daydream
{
	OpenGLSwapchain::OpenGLSwapchain(const DaydreamWindow& _window, const SwapchainDesc& _desc)
		:Swapchain(_desc)
	{
		desc = _desc;
		window = (GLFWwindow*)_window.GetNativeWindow();
		

	}
	OpenGLSwapchain::~OpenGLSwapchain()
	{
	}
	void OpenGLSwapchain::SetVSync(bool _enabled)
	{
		glfwSwapInterval(desc.isVSync);
	}

	void OpenGLSwapchain::Present()
	{
		glfwSwapBuffers(window);
	}
	//void OpenGLSwapchain::ResizeSwapchain(UInt32 _width, UInt32 _height)
	//{
	//	glViewport(0, 0, _width, _height);
	//}
	void OpenGLSwapchain::BeginFrame()
	{
		GLFWwindow* currentContext = glfwGetCurrentContext();
		if (currentContext != window)
		{
			glfwMakeContextCurrent(window);
		}
	}
	void OpenGLSwapchain::EndFrame()
	{
		glfwMakeContextCurrent(nullptr);
	}
}
