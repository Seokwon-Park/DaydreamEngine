#pragma once

#include "Steins/Render/Swapchain.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	class OpenGLSwapchain : public SwapChain
	{
	public:
		OpenGLSwapchain(SwapChainDesc* _desc, SteinsWindow* _window);
		virtual ~OpenGLSwapchain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* window;
		
	};
}