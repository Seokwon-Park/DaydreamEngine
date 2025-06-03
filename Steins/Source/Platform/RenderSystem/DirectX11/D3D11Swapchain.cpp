#include "SteinsPCH.h"
#include "D3D11SwapChain.h"

#include "Steins/Core/Window.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	
	D3D11SwapChain::D3D11SwapChain(D3D11GraphicsDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		device = _device;
		desc = *_desc;
		STEINS_CORE_ASSERT(device, "Device is nullptr");

		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;    // 샘플 수 (1이면 MSAA 비활성화)
		sampleDesc.Quality = 0;  // 품질 레벨 (0이면 기본값)

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = _desc->width;
		bufferDesc.Height = _desc->height;
		//bufferDesc.RefreshRate 
		bufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 프로그레시브로 설정
		bufferDesc.Format = GraphicsUtil::RenderFormatToDXGIFormat(_desc->format);

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc = bufferDesc;
		desc.SampleDesc = sampleDesc;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = _desc->bufferCount;
		desc.OutputWindow = glfwGetWin32Window((GLFWwindow*)_window->GetNativeWindow());
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = 0;

		HRESULT hr = device->GetFactory()->CreateSwapChain(device->GetDevice(), &desc, swapChain.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed To Create Swapchain!");
		device->AddSwapChain(this);

		//backFramebuffer = MakeShared<D3D11Framebuffer>(device, this);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Cast<FLOAT>(_desc->width);
		viewport.Height= Cast<FLOAT>(_desc->height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		device->GetContext()->RSSetViewports(1, &viewport);

	}
	D3D11SwapChain::~D3D11SwapChain()
	{
		device = nullptr;
	}
	void D3D11SwapChain::SetVSync(bool _enabled)
	{
	}
	void D3D11SwapChain::SwapBuffers()
	{
		swapChain->Present(desc.isVSync, 0);
	}

}