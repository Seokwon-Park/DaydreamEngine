#pragma once

#include "Steins/Render/Swapchain.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	class OpenGLSwapChain : public SwapChain
	{
	public:
		OpenGLSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window);
		virtual ~OpenGLSwapChain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;

		virtual Framebuffer* GetBackFramebuffer() { return nullptr; };
	private:
		GLFWwindow* window;
		
	};
}