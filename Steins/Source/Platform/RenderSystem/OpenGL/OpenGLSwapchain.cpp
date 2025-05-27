#include "SteinsPCH.h"
#include "OpenGLSwapChain.h"

#include "Steins/Core/Window.h"
#include "GLFW/glfw3.h"

#include "glad/glad.h"


namespace Steins
{
	OpenGLSwapChain::OpenGLSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		desc = *_desc;
		window = (GLFWwindow*)_window->GetNativeWindow();
		glfwMakeContextCurrent(window);
	}
	OpenGLSwapChain::~OpenGLSwapChain()
	{
	}
	void OpenGLSwapChain::SetVSync(bool _enabled)
	{
		glfwSwapInterval(desc.isVSync);
	}

	void OpenGLSwapChain::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}
}
