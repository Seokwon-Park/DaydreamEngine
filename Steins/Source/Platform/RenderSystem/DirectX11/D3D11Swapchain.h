#pragma once

#include "Steins/Render/SwapChain.h"
#include "D3D11RenderDevice.h"
#include "D3D11Framebuffer.h"

namespace Steins
{
	class D3D11SwapChain : public SwapChain
	{
	public:
		D3D11SwapChain(D3D11RenderDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window);
		virtual ~D3D11SwapChain();

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapChain(UInt32 _width, UInt32 _height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Framebuffer* GetBackFramebuffer() { return framebuffer.get(); };

		inline IDXGISwapChain* GetDXGISwapChain() { return swapChain.Get(); }
		
	private:
		D3D11RenderDevice* device;
		ComPtr<IDXGISwapChain> swapChain;

		Shared<D3D11Framebuffer> framebuffer;
	};
}