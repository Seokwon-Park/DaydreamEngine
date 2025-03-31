#pragma once

#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	class D3D12GraphicsDevice : public GraphicsDevice
	{
	public:

		D3D12GraphicsDevice(SteinsWindow* _window);
		virtual ~D3D12GraphicsDevice();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	};
}

