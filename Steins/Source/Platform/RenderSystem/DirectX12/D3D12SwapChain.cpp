#include "SteinsPCH.h"
#include "D3D12SwapChain.h"

#include "Platform/RenderSystem/DXHelper.h"

#include "Steins/Core/Window.h"
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace Steins
{
	D3D12SwapChain::D3D12SwapChain(GraphicsDevice* _device, SwapchainDesc* _desc, SteinsWindow* _window)
	{
		device = Cast<D3D12GraphicsDevice>(_device);
		desc = *_desc;
		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;    // 샘플 수 (1이면 MSAA 비활성화)
		sampleDesc.Quality = 0;  // 품질 레벨 (0이면 기본값)

		DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
		ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
		swapchainDesc.Width = _desc->width;
		swapchainDesc.Height = _desc->height;
		swapchainDesc.Format = DXHelper::RenderFormatToDXGIFormat(_desc->format);
		swapchainDesc.Stereo = 0;
		swapchainDesc.SampleDesc = sampleDesc;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.BufferCount = _desc->bufferCount;
		swapchainDesc.Scaling = DXGI_SCALING_NONE;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapchainDesc.Flags = 0;

		ComPtr<IDXGISwapChain1> swapChain1;
		HRESULT hr = device->GetFactory()->CreateSwapChainForHwnd(
			device->GetCommandQueue(),
			glfwGetWin32Window((GLFWwindow*)_window->GetNativeWindow()),
			&swapchainDesc,
			nullptr, nullptr,
			swapChain1.GetAddressOf()
		);

		for (UINT i = 0; i < swapchainDesc.BufferCount; i++)
		{
			HRESULT hr = device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fences[i].GetAddressOf()));
			fenceValues[i] = 0; // set the initial fence value to 0
		}

		swapChain1->QueryInterface(swapChain.GetAddressOf());

		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create swapchain!");



	}
	D3D12SwapChain::~D3D12SwapChain()
	{
		device = nullptr;
	}
	void D3D12SwapChain::SetVSync(bool _enabled)
	{
	}

	void D3D12SwapChain::SwapBuffers()
	{
		swapChain->Present(desc.isVSync, 0);

		int frameIndex = swapChain->GetCurrentBackBufferIndex();

		// GPU가 이전 프레임 작업을 끝낼 때까지 기다림
		if (fences[frameIndex]->GetCompletedValue() < fenceValues[frameIndex])
		{
			fences[frameIndex]->SetEventOnCompletion(fenceValues[frameIndex], device->GetFenceEvent());
			WaitForSingleObject(device->GetFenceEvent(), INFINITE);
		}

		fenceValues[frameIndex]++;
		device->SignalFence(fences[frameIndex].Get(), fenceValues[frameIndex]);
	}
}
