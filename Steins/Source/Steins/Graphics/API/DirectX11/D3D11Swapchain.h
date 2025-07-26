#pragma once

#include "Steins/Graphics/Core/Swapchain.h"
#include "D3D11RenderDevice.h"
#include "D3D11Framebuffer.h"

namespace Steins
{
	class D3D11Swapchain : public Swapchain
	{
	public:
		D3D11Swapchain(D3D11RenderDevice* _device, SteinsWindow* _window, const SwapchainDesc& _desc);
		virtual ~D3D11Swapchain();

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 _height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Shared<Framebuffer> GetBackFramebuffer() { return framebuffer; };

		inline IDXGISwapChain* GetDXGISwapchain() { return swapChain.Get(); }
		
	private:
		D3D11RenderDevice* device;
		ComPtr<IDXGISwapChain> swapChain;

		Shared<D3D11Framebuffer> framebuffer;
	};
}