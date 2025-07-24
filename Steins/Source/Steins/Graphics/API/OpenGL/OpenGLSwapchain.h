#pragma once

#include "Steins/Graphics/Core/Swapchain.h"

#include "GLFW/glfw3.h"

namespace Steins
{
	class OpenGLSwapchain : public Swapchain
	{
	public:
		OpenGLSwapchain(SteinsWindow* _window, const SwapchainDesc& _desc);
		virtual ~OpenGLSwapchain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Framebuffer* GetBackFramebuffer() { return nullptr; };
	private:
		GLFWwindow* window;
		
	};
}