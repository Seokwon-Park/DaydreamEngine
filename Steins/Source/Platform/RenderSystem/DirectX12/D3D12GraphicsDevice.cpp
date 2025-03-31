#include "SteinsPCH.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	D3D12GraphicsDevice::D3D12GraphicsDevice(SteinsWindow* _window)
		:GraphicsDevice(_window)
	{
	}

	D3D12GraphicsDevice::~D3D12GraphicsDevice()
	{
	}

	void D3D12GraphicsDevice::Init()
	{
		// DXGI ���丮 ����
		CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

		// ����̽� ����
		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		dxgiFactory->EnumAdapters1(0, &adapter);
		D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

		// Ŀ�ǵ� ť ����
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));

		//// ���� ü�� ����
		//DXGI_SWAP_CHAIN_DESC swapDesc = {};
		//swapDesc.BufferCount = 2;
		//swapDesc.BufferDesc.Width = 800;
		//swapDesc.BufferDesc.Height = 600;
		//swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//swapDesc.OutputWindow = hwnd;
		//swapDesc.Windowed = TRUE;
		//swapDesc.SampleDesc.Count = 1;
		//dxgiFactory->CreateSwapChain(commandQueue.Get(), &swapDesc, &swapChain);

	}

	void D3D12GraphicsDevice::Shutdown()
	{
	}

	void D3D12GraphicsDevice::Render()
	{
	}

	void D3D12GraphicsDevice::SwapBuffers()
	{
	}
}
