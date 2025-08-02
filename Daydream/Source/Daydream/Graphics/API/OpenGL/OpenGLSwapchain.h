#pragma once

#include "Daydream/Graphics/Core/Swapchain.h"
#include "OpenGLFramebuffer.h"

#include "GLFW/glfw3.h"

namespace Daydream
{
	class OpenGLSwapchain : public Swapchain
	{
	public:
		OpenGLSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc);
		virtual ~OpenGLSwapchain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 _height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Shared<Framebuffer> GetBackFramebuffer() { return framebuffer; };
	private:
		GLFWwindow* window;
		Shared<OpenGLFramebuffer> framebuffer;
		
	};
}