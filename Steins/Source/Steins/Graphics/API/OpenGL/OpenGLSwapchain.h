#pragma once

#include "Steins/Graphics/Core/SwapChain.h"

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
		virtual void ResizeSwapChain(UInt32 _width, UInt32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Framebuffer* GetBackFramebuffer() { return nullptr; };
	private:
		GLFWwindow* window;
		
	};
}