#pragma once
#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

namespace Daydream
{
	class DaydreamWindow;
	struct SwapchainDesc
	{
		UInt32 width = 0;
		UInt32 height = 0;
		UInt32 bufferCount = 2;
		RenderFormat format = RenderFormat::R8G8B8A8_UNORM;
		bool isFullscreen = false;
		bool isVSync = false;
	};

	class Swapchain
	{
	public:
		virtual ~Swapchain() = default;

		bool GetVSync() const { return desc.isVSync; }
		virtual void SetVSync(bool _enabled) = 0;

		virtual void Present() = 0;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 height) = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		void ResizeFramebuffers();
		void BeginRenderPass() { return mainRenderPass->Begin(GetBackFramebuffer()); }
		void EndRenderPass() { return mainRenderPass->End(); }

		virtual Shared<Framebuffer> GetBackFramebuffer() = 0;
		static Shared<Swapchain> Create(DaydreamWindow* _window, const SwapchainDesc& _desc);
	protected:
		SwapchainDesc desc;
		Shared<RenderPass> mainRenderPass;
	};
}