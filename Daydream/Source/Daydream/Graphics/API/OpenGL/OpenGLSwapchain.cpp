#include "DaydreamPCH.h"
#include "OpenGLSwapchain.h"
#include "OpenGLRenderPass.h"

#include "Daydream/Core/Window.h"
#include "GLFW/glfw3.h"

#include "glad/glad.h"


namespace Daydream
{
	OpenGLSwapchain::OpenGLSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		desc = _desc;
		window = (GLFWwindow*)_window->GetNativeWindow();
		glfwMakeContextCurrent(window);

		RenderPassAttachmentDesc colorDesc;
		colorDesc.format = _desc.format;
		colorDesc.isSwapchain = true;
		RenderPassDesc rpDesc{};
		rpDesc.colorAttachments.push_back(colorDesc);
		

		mainRenderPass = MakeShared<OpenGLRenderPass>(rpDesc);

		framebuffer = MakeShared<OpenGLFramebuffer>(this);
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
		ResizeFramebuffers();
	}
	void OpenGLSwapchain::ResizeSwapchain(UInt32 _width, UInt32 _height)
	{
		glViewport(0, 0, _width, _height);
	}
	void OpenGLSwapchain::BeginFrame()
	{
	}
	void OpenGLSwapchain::EndFrame()
	{
	}
}
