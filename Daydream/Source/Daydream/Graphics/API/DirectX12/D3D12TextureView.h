#pragma once

#include "Daydream/Graphics/Resources/TextureView.h"

namespace Daydream
{

	class D3D12TextureView : public TextureView
	{
	public:
		explicit D3D12TextureView(const TextureViewDesc& _desc);
		~D3D12TextureView() = default;

		const TextureViewDesc& GetDesc() const override { return desc; }

		void SetCPUHandle(D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle) { cpuHandle = _cpuHandle; }
		void SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle) { gpuHandle = _gpuHandle; }

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const { return cpuHandle; }
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return gpuHandle; }

	private:
		TextureViewDesc desc;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {};
	};
}
