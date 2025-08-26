#include "DaydreamPCH.h"
#include "D3D11Swapchain.h"

#include "Daydream/Core/Window.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "D3D11Utility.h"

namespace Daydream
{
	
	D3D11Swapchain::D3D11Swapchain(D3D11RenderDevice* _device, DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		device = _device;
		desc = _desc;
		DAYDREAM_CORE_ASSERT(device, "Device is nullptr");

		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;    // 샘플 수 (1이면 MSAA 비활성화)
		sampleDesc.Quality = 0;  // 품질 레벨 (0이면 기본값)

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = _desc.width;
		bufferDesc.Height = _desc.height;
		//bufferDesc.RefreshRate 
		//bufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		//bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 프로그레시브로 설정
		bufferDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format);

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc = bufferDesc;
		desc.SampleDesc = sampleDesc;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = _desc.bufferCount;
		desc.OutputWindow = glfwGetWin32Window((GLFWwindow*)_window->GetNativeWindow());
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = device->GetFactory()->CreateSwapChain(device->GetDevice(), &desc, swapChain.GetAddressOf());
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed To Create Swapchain!");

		RenderPassAttachmentDesc colorDesc;
		colorDesc.format = _desc.format;
		colorDesc.isSwapchain = true;

		RenderPassDesc rpDesc{};
		rpDesc.colorAttachments.push_back(colorDesc);

		mainRenderPass = MakeShared<D3D11RenderPass>(device, rpDesc);

		framebuffer = MakeShared<D3D11Framebuffer>(device, mainRenderPass.get(), this);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Cast<Float32>(_desc.width);
		viewport.Height= Cast<Float32>(_desc.height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		device->GetContext()->RSSetViewports(1, &viewport);

	}
	D3D11Swapchain::~D3D11Swapchain()
	{
		device = nullptr;
	}
	void D3D11Swapchain::SetVSync(bool _enabled)
	{
	}
	void D3D11Swapchain::SwapBuffers()
	{
		swapChain->Present(desc.isVSync, 0);
		ResizeFramebuffers();
	}

	void D3D11Swapchain::ResizeSwapchain(UInt32 _width, UInt32 _height)
	{
		framebuffer.reset();
		swapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		framebuffer = MakeShared<D3D11Framebuffer>(device, mainRenderPass.get(), this);
	}

	void D3D11Swapchain::BeginFrame()
	{
	}

	void D3D11Swapchain::EndFrame()
	{
	}

}