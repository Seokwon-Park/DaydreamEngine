#pragma once
#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	struct SwapchainDesc
	{
		UInt32 width = 0;
		UInt32 height = 0;
		UInt32 buffer_count = 2;
		RenderFormat format = RenderFormat::R8G8B8A8_UNORM;
		bool isFullscreen = false;
		bool isVSync = false;
	};

	class Swapchain
	{
	public:
		virtual ~Swapchain() = default;

		bool GetVSync() const { return isVSync; }
		virtual void SetVSync(bool _enabled) = 0;

		virtual void SwapBuffers() = 0;

		static Shared<Swapchain> Create(SwapchainDesc* _desc);
	private:
		bool isVSync = false;
	};
}