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

		ComPtr<ID3D12Resource> m_renderTargets;
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;

		ComPtr<IDXGIFactory7> dxgiFactory;
		ComPtr<IDXGISwapChain> swapChain;

		ComPtr<ID3D12Debug> debugLayer;

		ComPtr<ID3D12Fence> m_fence;
	};
}

