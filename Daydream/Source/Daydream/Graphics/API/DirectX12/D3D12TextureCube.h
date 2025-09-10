#pragma once

#include "Daydream/Graphics/Resources/TextureCube.h"
#include "D3D12RenderDevice.h"
#include "D3D12Sampler.h"

namespace Daydream
{
	class D3D12TextureCube : public TextureCube
	{
	public:
		// Constrcuter Destructer
		D3D12TextureCube(D3D12RenderDevice* _device, const TextureDesc& _desc);
		virtual ~D3D12TextureCube();

		virtual void SetSampler(Shared<Sampler> _sampler) override;

		virtual inline void* GetNativeHandle() override { return texture.Get(); }

		ID3D12Resource* GetID3D12Resource() { return texture.Get(); }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle() { return srvCpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle() { return srvGpuHandle; }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandle() { return rtvCpuHandle; }
		inline const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSVCPUHandle() { return dsvCpuHandle; }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetUAVCPUHandle() { return uavCpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle() { return uavGpuHandle; }
		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerHandle() { return textureSampler->GetSamplerHandle(); }

	private:
		D3D12RenderDevice* device;
		D3D12Sampler* textureSampler;

		ComPtr<ID3D12Resource> texture;

		D3D12_CPU_DESCRIPTOR_HANDLE samplerCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE samplerGpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = {};

		D3D12_CPU_DESCRIPTOR_HANDLE uavCpuHandle = {};
		D3D12_GPU_DESCRIPTOR_HANDLE uavGpuHandle = {};
	};
}
