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

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) override;
	private:
		ComPtr<IDXGISwapChain3> swapChain;

		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<ID3D12CommandAllocator> commandAllocator;

		ComPtr<ID3D12Resource> renderTargets;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12DescriptorHeap> rtvHeap;
		ComPtr<ID3D12PipelineState> pipelineState;
		ComPtr<ID3D12GraphicsCommandList> commandList;

		ComPtr<IDXGIFactory7> dxgiFactory;
		ComPtr<ID3D12Debug> debugLayer;

		ComPtr<ID3D12Fence> fence;
	};
}

