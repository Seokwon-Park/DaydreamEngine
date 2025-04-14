#pragma once

#include "Steins/Render/Swapchain.h"

namespace Steins
{
	class OpenGLSwapchain : public Swapchain
	{
	public:
		OpenGLSwapchain(SwapchainDesc* _desc, SteinsWindow* _window);
		virtual ~OpenGLSwapchain() override;
	private:

		// Inherited via Swapchain
		void SetVSync(bool _enabled) override;
		void SwapBuffers() override;
	};
}